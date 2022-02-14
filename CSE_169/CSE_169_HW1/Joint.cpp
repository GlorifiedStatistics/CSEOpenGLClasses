#include "Joint.h"

void Joint::init() {
	// Need to immediately constrain pose
	pose = constrainDOF(pose);

	mesh = genMesh(boxmin, boxmax);

	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(GLfloat), mesh.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Joint::drawMe(glm::mat4 M, GLuint shaderProgram) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(M));
	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, mesh.size() / 3);
}

void Joint::draw(glm::mat4 C, GLuint shaderProgram) {
	glm::mat4 R = _R(pose.z, Z_AXIS) * _R(pose.y, Y_AXIS) * _R(pose.x, X_AXIS);
	glm::mat4 M = C * glm::translate(glm::mat4(1), offset) * R;
	drawMe(M, shaderProgram);
	for (Joint* j : children) j->draw(M, shaderProgram);
}