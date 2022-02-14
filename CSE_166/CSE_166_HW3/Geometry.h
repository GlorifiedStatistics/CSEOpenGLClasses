#pragma once
#include "Node.h"

class Geometry : public Node {
private:
	OBJ shape;
	glm::vec3 color = glm::vec3(1, 0, 0);
	Material material = METAL;
	unsigned int drawMode = DRAW_PHONG;

	GLuint VAO, VBO, NB;
	void buildBuffer();
public:
	Geometry(OBJ obj);
	Geometry(std::string loadPath);
	virtual void draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader);
	virtual void update();
	Geometry* set(unsigned int dm);
	Geometry* set(glm::vec3 newColor);
	Geometry* set(Material mat);
	Geometry* set(unsigned int dm, glm::vec3 newColor);
	Geometry* set(unsigned int dm, Material mat);
	Geometry* set(glm::vec3 newColor, Material mat);
	Geometry* set(unsigned int dm, glm::vec3 newColor, Material mat);
};