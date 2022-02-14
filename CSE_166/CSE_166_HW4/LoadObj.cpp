#include "LoadObj.h"

void getPointsAndNormals(std::string a, std::string b, std::string c, std::vector<unsigned int>& facesPoints, std::vector<unsigned int>& facesNormals);


// Code taken from the writeup
OBJ loadObj(std::string filename) {
	std::ifstream objFile(filename); // The obj file we are reading.
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> facesPoints;
	std::vector<unsigned int> facesNormals;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}

			else if (label == "vn") {
				glm::vec3 norm;
				ss >> norm.x >> norm.y >> norm.z;

				// Process the point. For example, you can save it to a.
				normals.push_back(glm::normalize(norm));
			}

			else if (label == "f") {
				std::string a, b, c;
				ss >> a >> b >> c;

				getPointsAndNormals(a, b, c, facesPoints, facesNormals);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << filename << std::endl;
	}

	objFile.close();

	std::vector<GLfloat> triangleMesh;

	for (unsigned int i = 0; i < facesPoints.size(); i++) {
		triangleMesh.push_back(points[facesPoints[i] - 1].x);
		triangleMesh.push_back(points[facesPoints[i] - 1].y);
		triangleMesh.push_back(points[facesPoints[i] - 1].z);
	}

	std::vector<GLfloat> triangleMeshNormals;

	for (unsigned int i = 0; i < facesNormals.size(); i++) {
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].x);
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].y);
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].z);
	}

	return OBJ(triangleMesh, triangleMeshNormals);
}


int getStringIndex(std::string s, int index) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] == '/') {
			if (index == 0) return i;
			index -= 1;
		}
	}
	return s.length();
}


void getPointsAndNormals(std::string a, std::string b, std::string c, std::vector<unsigned int>& facesPoints, std::vector<unsigned int>& facesNormals) {
	facesPoints.push_back(std::stoi(a.substr(0, getStringIndex(a, 0))));
	facesNormals.push_back(std::stoi(a.substr(getStringIndex(a, 1) + 1)));
	facesPoints.push_back(std::stoi(b.substr(0, getStringIndex(b, 0))));
	facesNormals.push_back(std::stoi(b.substr(getStringIndex(b, 1) + 1)));
	facesPoints.push_back(std::stoi(c.substr(0, getStringIndex(c, 0))));
	facesNormals.push_back(std::stoi(c.substr(getStringIndex(c, 1) + 1)));
}

OBJ genSphere(float size) {
	OBJ sphere = loadObj(SPHERE_OBJ_PATH);

	int mx = 100, my = 100, mz = 100, Mx = -100, My = -100, Mz = -100;

	for (unsigned int i = 0; i < sphere.getPoints().size(); i += 3) {
		int x = sphere.getPoints()[i];
		int y = sphere.getPoints()[i + 1];
		int z = sphere.getPoints()[i + 2];

		if (x < mx) mx = x;
		if (x > Mx) Mx = x;
		if (y < my) my = y;
		if (y > My) My = y;
		if (z < mz) mz = z;
		if (z > Mz) Mz = z;
	}

	std::vector<GLfloat> points;
	for (unsigned int i = 0; i < sphere.getPoints().size(); i++) {
		points.push_back(sphere.getPoints()[i] * size);
	}

	return OBJ(points, sphere.getNormals());
}
