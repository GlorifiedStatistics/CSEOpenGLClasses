#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

class OBJ {
private:
	std::vector<GLfloat> points, normals;
public:
	OBJ();
	OBJ(std::vector<GLfloat> points, std::vector<GLfloat> normals);
	OBJ(std::vector<glm::vec3> points, std::vector<glm::vec3> normals);
	OBJ stretch(float x, float y, float z);
	OBJ stretch(glm::vec3 vals);
	OBJ stretch(float s);
	OBJ rotate(float angle, glm::vec3 axis);
	std::vector<GLfloat> getPoints();
	std::vector<GLfloat> getNormals();
	unsigned int size();
};