#pragma once
#include "Node.h"
#include "LoadObj.h"

class Skybox : public Node {
private:
	OBJ box;
	unsigned int skyboxId;
	GLuint VAO, VBO, NB;
public:
	Skybox(std::vector<std::string> faces);
	virtual void draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader);
	virtual void update();
};