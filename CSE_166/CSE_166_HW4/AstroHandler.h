#pragma once
#include "Astro.h"

class AstroHandler {
private:
	std::vector<Astro*> astros;
	long randomWaitTime = 0, lastIncommingCharacter = 0;
public:
	AstroHandler(unsigned int numVerts, GLuint VAO);
	~AstroHandler();
	void draw(GLuint shader);
	void update(glm::vec3 mainCharacterPos);
	glm::vec3 collide(glm::vec3 point);
};