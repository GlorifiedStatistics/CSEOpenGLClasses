#pragma once
#include "Node.h"

#define FLOOR_SIZE 64

class Floor : public Node {
private:
	OBJ floor;
	unsigned int floorId;
	GLuint VAO, VBO, NB, TC;
public:
	Floor(std::string loadPath);
	virtual void draw(glm::mat4 C, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, GLuint shader);
	virtual void update();
};