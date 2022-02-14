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


std::vector<GLfloat> makeQuadTriangles(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
	std::vector<GLfloat> ret{
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,

		p3.x, p3.y, p3.z,
		p4.x, p4.y, p4.z,
		p1.x, p1.y, p1.z
	};
	return ret;
}

// Generate the tesselated sphere
OBJ genDiscoSphere() {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	for (unsigned i = 0; i <= DISCO_SPHERE_STACK_COUNT; i++) {
		float lat0 = pi * (-0.5f + (float)(i - 1) / DISCO_SPHERE_STACK_COUNT);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = pi * (-0.5f + (float)i / DISCO_SPHERE_STACK_COUNT);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);
		for (unsigned j = 0; j <= DISCO_SPHERE_SECTOR_COUNT; j++) {
			float lng = 2 * pi * (float)(j - 1) / DISCO_SPHERE_SECTOR_COUNT;
			float lng1 = 2 * pi * (float)(j) / DISCO_SPHERE_SECTOR_COUNT;

			float x1 = cos(lng1);
			float y1 = sin(lng1);
			float x = cos(lng);
			float y = sin(lng);

			glm::vec3 p1(x * zr1, y * zr1, z1);
			glm::vec3 p2(x * zr0, y * zr0, z0);
			glm::vec3 p3(x1 * zr0, y1 * zr0, z0);
			glm::vec3 p4(x1 * zr1, y1 * zr1, z1);
			for (GLfloat f : makeQuadTriangles(p1, p2, p3, p4))
				vertices.push_back(f);

			float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
			float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
			float z_avg = (2 * z0 + 2 * z1) / 4;

			glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

			for (unsigned int i = 0; i < 6; i++) {
				normals.push_back(avg.x);
				normals.push_back(avg.y);
				normals.push_back(avg.z);
			}
		}
	}

	return OBJ(vertices, normals);
}


void _genSideFace(std::vector<GLfloat>& points, std::vector<GLfloat>& normals, float size, bool right) {
	int mult = 1;
	if (!right) mult = -1;

	glm::vec3 p1, p2, p3, p4;

	if (right) {
		p1 = glm::vec3(mult * size, -size, size);
		p2 = glm::vec3(mult * size, size, size);
		p3 = glm::vec3(mult * size, size, -size);
		p4 = glm::vec3(mult * size, -size, -size);
	}
	else {
		p4 = glm::vec3(mult * size, -size, size);
		p3 = glm::vec3(mult * size, size, size);
		p2 = glm::vec3(mult * size, size, -size);
		p1 = glm::vec3(mult * size, -size, -size);
	}

	for (GLfloat f : makeQuadTriangles(p1, p2, p3, p4))
		points.push_back(f);

	for (unsigned int i = 0; i < 6; i++) {
		normals.push_back(mult);
		normals.push_back(0);
		normals.push_back(0);
	}
}

void _genTopFace(std::vector<GLfloat>& points, std::vector<GLfloat>& normals, float size, bool top) {
	int mult = 1;
	if (!top) mult = -1;

	glm::vec3 p1, p2, p3, p4;

	if (top) {
		p1 = glm::vec3(-size, mult * size, size);
		p2 = glm::vec3(-size, mult * size, -size);
		p3 = glm::vec3(size, mult * size, -size);
		p4 = glm::vec3(size, mult * size, size);
	}
	else {
		p4 = glm::vec3(-size, mult * size, size);
		p3 = glm::vec3(-size, mult * size, -size);
		p2 = glm::vec3(size, mult * size, -size);
		p1 = glm::vec3(size, mult * size, size);
	}
	

	for (GLfloat f : makeQuadTriangles(p1, p2, p3, p4))
		points.push_back(f);

	for (unsigned int i = 0; i < 6; i++) {
		normals.push_back(0);
		normals.push_back(mult);
		normals.push_back(0);
	}
}

void _genFrontFace(std::vector<GLfloat>& points, std::vector<GLfloat>& normals, float size, bool front) {
	int mult = -1;
	if (!front) mult = 1;

	glm::vec3 p1, p2, p3, p4;

	if (!front) {
		p1 = glm::vec3(-size, -size, mult * size);
		p2 = glm::vec3(-size, size, mult * size);
		p3 = glm::vec3(size, size, mult * size);
		p4 = glm::vec3(size, -size, mult * size);
	}
	else {
		p4 = glm::vec3(-size, -size, mult * size);
		p3 = glm::vec3(-size, size, mult * size);
		p2 = glm::vec3(size, size, mult * size);
		p1 = glm::vec3(size, -size, mult * size);
	}

	for (GLfloat f : makeQuadTriangles(p1, p2, p3, p4))
		points.push_back(f);

	for (unsigned int i = 0; i < 6; i++) {
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-mult);
	}
}

// Generate in order: right, left, top, bottom, front, back
//   from looking at -z
// Size is distance from center in both dirs
// Normals face outwards
OBJ genSkybox() {
	std::vector<GLfloat> points;
	std::vector<GLfloat> normals;

	// The right and left faces
	_genSideFace(points, normals, SKYBOX_SIZE, true);
	_genSideFace(points, normals, SKYBOX_SIZE, false);

	// The top and bottom faces
	_genTopFace(points, normals, SKYBOX_SIZE, true);
	_genTopFace(points, normals, SKYBOX_SIZE, false);

	// The front and back faces
	_genFrontFace(points, normals, SKYBOX_SIZE, true);
	_genFrontFace(points, normals, SKYBOX_SIZE, false);

	return OBJ(points, normals);
}

OBJ genTriangle() {
	std::vector<GLfloat> points = {
		0, 0, 0,
		5, 0, 0,
		2.5, 5, 0
	};
	std::vector<GLfloat> normals = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
	};
	return OBJ(points, normals);
}

OBJ genFloor() {
	std::vector<GLfloat> points;
	std::vector<GLfloat> normals;

	_genTopFace(points, normals, FLOOR_SIZE, false);
	for (unsigned int i = 0; i < points.size(); i+=3) {
		points[i + 1] = 0;
	}
	return OBJ(points, normals);
}