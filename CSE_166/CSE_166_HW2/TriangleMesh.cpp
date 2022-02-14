#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::string filePath, MaterialProperties matProps, Light& lightObj, int* normalMode, int windowWidth, int windowHeight) {

	OBJ everything = loadObj(filePath, windowWidth, windowHeight);

	triangleMesh = everything.mesh;
	triangleMeshNormals = everything.normals;

	materialProperties = matProps;

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	lightModel = glm::mat4(1);
	nm = normalMode;

	light = lightObj;

	setBufferData();
}

TriangleMesh::~TriangleMesh() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void TriangleMesh::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader, "mode"), *nm);

	// Set the material properties
	glUniform3fv(glGetUniformLocation(shader, "matProps.ambient"), 1, glm::value_ptr(materialProperties.ambient));
	glUniform3fv(glGetUniformLocation(shader, "matProps.diffuse"), 1, glm::value_ptr(materialProperties.diffuse));
	glUniform3fv(glGetUniformLocation(shader, "matProps.specular"), 1, glm::value_ptr(materialProperties.specular));
	glUniform1f(glGetUniformLocation(shader, "matProps.shininess"), materialProperties.shininess);

	// Set the lighting info
	glm::vec3 lp = lightModel * glm::vec4(light.pos, 1);
	glUniform3fv(glGetUniformLocation(shader, "light.pos"), 1, glm::value_ptr(lp));
	glUniform3fv(glGetUniformLocation(shader, "light.intensities"), 1, glm::value_ptr(light.intensities));
	glUniform1f(glGetUniformLocation(shader, "light.attenuation"), light.attenuation);
	glUniform1f(glGetUniformLocation(shader, "light.ambientCoefficient"), light.ambientCoefficient);
	glUniform1f(glGetUniformLocation(shader, "light.exponent"), light.exponent);


	// Bind the VAO
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Draw the points 
	glDrawArrays(GL_TRIANGLES, 0, triangleMesh.size() / 3);

	// Setup to draw the light
	glUniform3fv(glGetUniformLocation(shader, "matProps.ambient"), 1, glm::value_ptr(light.intensities));
	glUniform3fv(glGetUniformLocation(shader, "matProps.diffuse"), 1, glm::value_ptr(glm::vec3(0, 0, 0)));
	glUniform3fv(glGetUniformLocation(shader, "matProps.specular"), 1, glm::value_ptr(glm::vec3(0, 0, 0)));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform1f(glGetUniformLocation(shader, "light.ambientCoefficient"), light.ambientCoefficient);

	// Draw the light
	glDrawArrays(GL_TRIANGLES, triangleMesh.size() / 3, light.lightMesh.mesh.size() / 3);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void TriangleMesh::update() {
	//spin(0.005f);
}

void TriangleMesh::rotate(float angle, glm::vec3 axis) {
	if (axis.x != 0 || axis.y != 0 || axis.z != 0)
		model = glm::rotate(model, angle, axis);
}

void TriangleMesh::scaleModel(float scale) {
	model = model * glm::scale(glm::vec3(scale, scale, scale));
}
void TriangleMesh::scaleLight(float scale) {
	lightModel = lightModel * glm::scale(glm::vec3(scale, scale, scale));
}


void TriangleMesh::setBufferData() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	std::vector<GLfloat> allMesh, allNorms;
	for (unsigned int i = 0; i < triangleMesh.size(); i++) {
		allMesh.push_back(triangleMesh[i]);
	}
	for (unsigned int i = 0; i < light.lightMesh.mesh.size(); i++) {
		allMesh.push_back(light.lightMesh.mesh[i]);
	}

	for (unsigned int i = 0; i < triangleMeshNormals.size(); i++) {
		allNorms.push_back(triangleMeshNormals[i]);
	}
	for (unsigned int i = 0; i < light.lightMesh.normals.size(); i++) {
		allNorms.push_back(light.lightMesh.normals[i]);
	}

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allMesh.size() * sizeof(GLfloat), allMesh.data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	
	// Set up the normal buffers
	glGenBuffers(1, &NB);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glBufferData(GL_ARRAY_BUFFER, allNorms.size() * sizeof(GLfloat), allNorms.data(), GL_STATIC_DRAW);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glVertexAttribPointer(2, 3,	GL_FLOAT, GL_FALSE, 3*sizeof(float), NULL);
	

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

