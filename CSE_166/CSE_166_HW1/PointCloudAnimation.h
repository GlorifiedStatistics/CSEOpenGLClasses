#pragma once

#include "Object.h"
#include "PointUtils.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class PointCloudAnimation : public Object
{
private:
	std::vector<std::vector<glm::vec3>> clouds;
	std::vector<glm::vec3> currentCloud;
	std::vector<glm::vec3> extraPoints;
	glm::vec3 currentColor;
	unsigned int nextCloud, currentStep;

	GLuint VAO, VBO;
	GLfloat pointSize;

	void setBufferData();
	void interpolateNext();

public:
	PointCloudAnimation(std::vector<std::string> filePaths, int windowWidth, int windowHeight);
	~PointCloudAnimation();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
	void transitionTo(unsigned int idx);
};