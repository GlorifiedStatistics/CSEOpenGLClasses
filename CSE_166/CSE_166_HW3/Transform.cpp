#include "Transform.h"

Transform* Transform::addChild(Node* child) {
	childNodes.push_back(child);
	return this;
}

Transform* Transform::addObj(OBJ obj) {
	return addChild(new Geometry(obj));
}

Transform* Transform::translate(float x, float y, float z) { return translate(glm::vec3(x, y, z)); }
Transform* Transform::translate(glm::vec3 t) {
	M = glm::translate(t) * M;
	return this;
}

Transform* Transform::rotate(float angle, glm::vec3 axis) { return rotate(RotateInfo(angle, axis)); }
Transform* Transform::rotate(RotateInfo rotateInfo) {
	M = glm::rotate(M, rotateInfo.angle, rotateInfo.axis);
	return this;
}

Transform* Transform::rotateAbout(float angle, glm::vec3 center, glm::vec3 axis) {
	M = glm::translate(center) * (glm::rotate(I, angle, axis) * glm::inverse(glm::translate(center)));
	return this;
}

void Transform::update() {
	for (glm::mat4(*func)(glm::mat4) : animationFunctions) M = func(M);
	for (Node* child : childNodes) child->update();
}

void Transform::draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader) {
	for (Node* child : childNodes) child->draw(C*M, view, cameraPos, shader);
}

Transform* Transform::addAnimation(glm::mat4(*func)(glm::mat4)) {
	animationFunctions.push_back(func);
	return this;
}