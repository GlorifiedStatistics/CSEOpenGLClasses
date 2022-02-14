#include "Geometry.h"

Geometry::Geometry(OBJ obj) {
	shape = obj;
	buildBuffer();
}

Geometry::Geometry(std::string loadPath) {
	shape = loadObj(loadPath);
	buildBuffer();
}

void Geometry::buildBuffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(GLfloat), shape.getPoints().data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


	// Set up the normal buffers
	glGenBuffers(1, &NB);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(GLfloat), shape.getNormals().data(), GL_STATIC_DRAW);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader) {
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));
	glUniform1i(glGetUniformLocation(shader, "mode"), drawMode);

	if (drawMode == DRAW_PHONG) {
		// Set the material properties
		glUniform3fv(glGetUniformLocation(shader, "matProps.ambient"), 1, glm::value_ptr(material.ambient));
		glUniform3fv(glGetUniformLocation(shader, "matProps.diffuse"), 1, glm::value_ptr(material.diffuse));
		glUniform3fv(glGetUniformLocation(shader, "matProps.specular"), 1, glm::value_ptr(material.specular));
		glUniform1f(glGetUniformLocation(shader, "matProps.shininess"), material.shininess);
	}
	else if (drawMode == DRAW_COLOR) {
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	}

	// Bind the VAO
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, shape.size() / 3);
}

void Geometry::update() {}

Geometry* Geometry::set(unsigned int dm) {
	drawMode = dm;
	return this;
}

Geometry* Geometry::set(glm::vec3 newColor) {
	color = newColor;
	return this;
}

Geometry* Geometry::set(Material mat) {
	material = mat;
	return this;
}

Geometry* Geometry::set(unsigned int dm, glm::vec3 newColor) { return this->set(dm)->set(newColor); }
Geometry* Geometry::set(glm::vec3 newColor, Material mat) { return this->set(newColor)->set(mat); }
Geometry* Geometry::set(unsigned int dm, Material mat) { return this->set(dm)->set(mat); }
Geometry* Geometry::set(unsigned int dm, glm::vec3 newColor, Material mat) { return this->set(dm)->set(newColor)->set(mat); }
