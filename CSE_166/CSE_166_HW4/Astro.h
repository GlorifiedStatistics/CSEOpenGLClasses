#pragma once
#include "Constants.h"
#include "Collision.h"

class Astro {
private:
	unsigned int numVerts;
	GLuint VAO;
	glm::vec3 pos = glm::vec3(0, FLOOR_HEIGHT, 0);
	float lookingRad = 0;

	long existTime = 0, initTime = 0, waitInitTime = 0, waitTime = 0;
	int id = 0;

public:
	unsigned int drawable = 0;

	Astro(unsigned int numVerts, GLuint VAO, int id) : numVerts(numVerts), VAO(VAO), id(id) {}
	virtual void draw(GLuint shader, AstroColor astroColor);
	virtual void update(std::vector<Astro*> astros, glm::vec3 mainCharacterPos);
	void init(std::vector<Astro*> astros, glm::vec3 mainCharacterPos);
	glm::vec3 collideCharacters(std::vector<Astro*> astros, glm::vec3 mainCharacterPos, glm::vec3 point);
	glm::vec3 collide(glm::vec3 point);
};