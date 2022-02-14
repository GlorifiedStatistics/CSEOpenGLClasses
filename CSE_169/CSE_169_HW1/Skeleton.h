#pragma once
#include "Constants.h"
#include "Joint.h"

class Skeleton {
private:
	Joint* root;
public:
	glm::mat4 M = glm::mat4(1);

	Skeleton(Joint* root) : root(root) {}
	void draw(GLuint shaderProgram) { root->draw(M, shaderProgram); }
};