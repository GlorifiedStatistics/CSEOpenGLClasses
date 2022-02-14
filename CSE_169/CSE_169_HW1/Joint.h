#pragma once

#include "Constants.h"

class Joint {
private:
	std::vector<GLfloat> mesh;
	std::vector<Joint*> children;
	glm::vec3 offset, boxmin, boxmax, pose;
	glm::vec2 rotxlimit, rotylimit, rotzlimit;

	GLuint VAO;
	void init();
	void drawMe(glm::mat4 C, GLuint shaderProgram);
public:
	Joint(glm::vec3 offset, glm::vec3 boxmin, glm::vec3 boxmax, glm::vec2 rotxlimit, glm::vec2 rotylimit, glm::vec2 rotzlimit, glm::vec3 pose)
		: offset(offset), boxmin(boxmin), boxmax(boxmax), rotxlimit(rotxlimit), rotylimit(rotylimit), rotzlimit(rotzlimit), pose(pose) { init(); }
	Joint(glm::vec3 offset, glm::vec3 boxmin, glm::vec3 boxmax, glm::vec2 rotxlimit, glm::vec2 rotylimit, glm::vec2 rotzlimit, glm::vec3 pose, std::vector<Joint*> children)
		: offset(offset), boxmin(boxmin), boxmax(boxmax), rotxlimit(rotxlimit), rotylimit(rotylimit), rotzlimit(rotzlimit), pose(pose), children(children) { init(); }
	void addChild(Joint* child) { children.push_back(child); }
	void draw(glm::mat4 C, GLuint shaderProgram);

	glm::vec3 constrainDOF(glm::vec3 dof) { return constrain(dof, rotxlimit, rotylimit, rotzlimit); }
	void setPose(glm::vec3 p) { pose = constrainDOF(p); }
	void setPoseX(float x) { pose = constrainDOF(glm::vec3(x, pose.y, pose.z)); }
	void setPoseY(float y) { pose = constrainDOF(glm::vec3(pose.x, y, pose.z)); }
	void setPoseZ(float z) { pose = constrainDOF(glm::vec3(pose.x, pose.y, z)); }
	void rotateX(float x) { pose = constrainDOF(glm::vec3(pose.x + x, pose.y, pose.z)); }
	void rotateY(float y) { pose = constrainDOF(glm::vec3(pose.x, pose.y + y, pose.z)); }
	void rotateZ(float z) { pose = constrainDOF(glm::vec3(pose.x, pose.y, pose.z + z)); }
};