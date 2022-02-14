#pragma once
#include "Node.h"
#include "Geometry.h"

class Transform: public Node {
private:
	glm::mat4 M;
	std::vector<Node*> childNodes;
	std::vector<glm::mat4(*)(glm::mat4)> animationFunctions;
public:
	Transform() : M(I) {}
	Transform(glm::mat4 m) : M(m) {}
	Transform* addChild(Node* child);
	Transform* addObj(OBJ obj);
	Transform* translate(float x, float y, float z);
	Transform* translate(glm::vec3 t);
	Transform* rotate(float angle, glm::vec3 axis);
	Transform* rotateAbout(float angle, glm::vec3 center, glm::vec3 axis);
	Transform* rotate(RotateInfo rotateInfo);
	Transform* addAnimation(glm::mat4(*func)(glm::mat4));
	virtual void draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader);
	virtual void update();
};