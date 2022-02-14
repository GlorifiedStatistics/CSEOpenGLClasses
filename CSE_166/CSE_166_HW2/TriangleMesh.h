#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LoadObj.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

struct MaterialProperties {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct Light {
	OBJ lightMesh;
	glm::vec3 pos;
	glm::vec3 intensities;
	GLfloat attenuation;
	GLfloat ambientCoefficient;
	GLfloat exponent;
};

const MaterialProperties emeraldProperties = {
	glm::vec3(0.0215, 0.3745, 0.0215),
	glm::vec3(0, 0, 0),
	glm::vec3(0.633, 0.727811, 0.633),
	0.5 * 128
};
const MaterialProperties bronzeProperties = {
	glm::vec3(0.2125, 0.1275, 0.054),
	glm::vec3(0.714, 0.4284, 0.18144),
	glm::vec3(0, 0, 0),
	0.2 * 128
};
const MaterialProperties brassProperties = {
	glm::vec3(0.329412, 0.223529, 0.027451),
	glm::vec3(0.780392, 0.568627, 0.113725),
	glm::vec3(0.992157, 0.941176, 0.807843),
	0.3 * 128
};

class TriangleMesh {
private:
	std::vector<GLfloat> triangleMesh;
	std::vector<GLfloat> triangleMeshNormals;
	MaterialProperties materialProperties;
	Light light;
	int* nm;

	GLuint VAO, VBO, NB;

	void setBufferData();

public:
	glm::mat4 model;
	glm::mat4 lightModel;
	TriangleMesh(std::string filePath, MaterialProperties materialProperties, Light& lightObj, int* normalMode, int windowWidth, int windowHeight);
	~TriangleMesh();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void rotate(float angle, glm::vec3 axis);
	void scaleModel(float scale);
	void scaleLight(float scale);
};