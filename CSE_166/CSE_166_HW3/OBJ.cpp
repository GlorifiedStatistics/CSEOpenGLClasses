#include "OBJ.h"

std::vector<GLfloat> toGLfloat(std::vector<glm::vec3> vals);
std::vector<glm::vec3> rotatePoints(std::vector<glm::vec3> points, float angle, glm::vec3 axis);
std::vector<glm::vec3> toVec3(std::vector<GLfloat> vals);

OBJ::OBJ() {}
OBJ::OBJ(std::vector<GLfloat> points, std::vector<GLfloat> normals) : points(points), normals(normals) {}
OBJ::OBJ(std::vector<glm::vec3> points, std::vector<glm::vec3> normals) : points(toGLfloat(points)), normals(toGLfloat(normals)) {}

OBJ OBJ::stretch(float x, float y, float z) { return stretch(glm::vec3(x, y, z)); }
OBJ OBJ::stretch(float s) { return stretch(glm::vec3(s, s, s)); }

OBJ OBJ::stretch(glm::vec3 vals) {
	std::vector<GLfloat> ret;
	for (unsigned int i = 0; i < size(); i++)ret.push_back(points[i]);
	for (unsigned int i = 0; i < size(); i += 3) {
		ret[i] *= vals.x;
		ret[i+1] *= vals.y;
		ret[i+2] *= vals.z;
	}
	return OBJ(ret, normals);
}

OBJ OBJ::rotate(float angle, glm::vec3 axis) {
	// Convert values to glm::vec3's, rotate both mesh and normals about point center, convert everything back to OBJ
	std::vector<glm::vec3> retPoints = toVec3(points), retNormals = toVec3(normals);
	retPoints = rotatePoints(retPoints, angle, axis);
	retNormals = rotatePoints(retNormals, angle, axis);
	return OBJ(toGLfloat(retPoints), toGLfloat(retNormals));
}

std::vector<GLfloat> OBJ::getPoints() { return points; }
std::vector<GLfloat> OBJ::getNormals() { return normals; }
unsigned int OBJ::size() { return points.size(); }



std::vector<glm::vec3> toVec3(std::vector<GLfloat> vals) {
	std::vector<glm::vec3> ret;
	for (unsigned int i = 0; i < vals.size(); i += 3)
		ret.push_back(glm::vec3(vals[i], vals[i + 1], vals[i + 2]));
	return ret;
}

std::vector<GLfloat> toGLfloat(std::vector<glm::vec3> vals) {
	std::vector<GLfloat> ret;
	for (unsigned int i = 0; i < vals.size(); i++) {
		ret.push_back(vals[i].x);
		ret.push_back(vals[i].y);
		ret.push_back(vals[i].z);
	}
	return ret;
}

std::vector<glm::vec3> rotatePoints(std::vector<glm::vec3> points, float angle, glm::vec3 axis) {
	std::vector<glm::vec3> ret;
	for (unsigned int i = 0; i < points.size(); i++)
		ret.push_back(glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(points[i], 1.0)));
	return ret;
}
