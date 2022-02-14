#include "LoadObj.h"



// Code taken from the writeup
OBJ loadObj(std::string filename, int windowWidth, int windowSize) {
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
	std::vector<glm::vec3> centeredPoints = centerAndResize(points, windowWidth, windowSize);

	std::vector<GLfloat> triangleMesh;
	
	for (unsigned int i = 0; i < facesPoints.size(); i++) {
		triangleMesh.push_back(centeredPoints[facesPoints[i] - 1].x);
		triangleMesh.push_back(centeredPoints[facesPoints[i] - 1].y);
		triangleMesh.push_back(centeredPoints[facesPoints[i] - 1].z);
	}

	std::vector<GLfloat> triangleMeshNormals;

	for (unsigned int i = 0; i < facesNormals.size(); i++) {
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].x);
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].y);
		triangleMeshNormals.push_back(normals[facesNormals[i] - 1].z);
	}

	OBJ ret;
	ret.mesh = triangleMesh;
	ret.normals = triangleMeshNormals;
	return ret;
}


OBJ loadLightObj(std::string path, int width, int height) {
	OBJ light = loadObj(path, width, height);

	// Convert to be 1/4 of current size
	for (unsigned int i = 0; i < light.mesh.size(); i++) {
		light.mesh[i] *= 0.125;
	}
	
	return light;
}


int getStringIndex(std::string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] == '/') return i;
	}
	return s.length();
}


void getPointsAndNormals(std::string a, std::string b, std::string c, std::vector<unsigned int>& facesPoints, std::vector<unsigned int>& facesNormals) {
	facesPoints.push_back(std::stoi(a.substr(0, getStringIndex(a))));
	facesNormals.push_back(std::stoi(a.substr(getStringIndex(a) + 2)));
	facesPoints.push_back(std::stoi(b.substr(0, getStringIndex(b))));
	facesNormals.push_back(std::stoi(b.substr(getStringIndex(b) + 2)));
	facesPoints.push_back(std::stoi(c.substr(0, getStringIndex(c))));
	facesNormals.push_back(std::stoi(c.substr(getStringIndex(c) + 2)));
}


// Gets the centers of the point cloud
glm::vec3 getCenters(std::vector<glm::vec3>& pc) {
	// Set the initial min and max values
	GLfloat minx = mx, miny = mx, minz = mx, maxx = mn, maxy = mn, maxz = mn;

	// Find min and max in each dim
	for (glm::vec3 p : pc) {
		if (p.x < minx) minx = p.x;
		if (p.x > maxx) maxx = p.x;

		if (p.y < miny) miny = p.y;
		if (p.y > maxy) maxy = p.y;

		if (p.z < minz) minz = p.z;
		if (p.z > maxz) maxz = p.z;
	}

	return glm::vec3((maxx + minx) / 2, (maxy + miny) / 2, (maxz + minz) / 2);
}



// Center the points, and resize them to the window
std::vector<glm::vec3> centerAndResize(std::vector<glm::vec3> pc, int width, int height) {

	// Center the points by finding the min and max values, and setting middle of each dimension
	glm::vec3 center = getCenters(pc);

	// Shift all the points by subtracting the center values
	for (unsigned int i = 0; i < pc.size(); i++) pc[i] -= center;

	// Get the largest absolute offset from the center
	GLfloat maxx = mn, maxy = mn, maxz = mn;
	for (glm::vec3 p : pc) {
		GLfloat x = abs(p.x), y = abs(p.y), z = abs(p.z);
		if (x > maxx) maxx = x;
		if (y > maxy) maxy = y;
		if (z > maxz) maxz = z;
	}

	// Find the ratio between the min window size and the max distance for each width/height
	GLfloat scalew = M_x / (maxx > maxz ? maxx : maxz);
	GLfloat scaleh = M_y / maxy;

	// Pick the smallest scale between the two because we want the whole object to fit into frame
	GLfloat scale = scalew < scaleh ? scalew : scaleh;

	// Scale all the points by multiplying by the scale in each direction
	for (unsigned int i = 0; i < pc.size(); i++) pc[i] *= scale;

	return pc;
}