#pragma once
#include "Constants.h"
#include "Collision.h"
#include "AstroHandler.h"

class MainCharacter {
private:
	glm::vec3 cameraPos = glm::vec3(0, 3, 20);			// Camera position.
	glm::mat4 view = glm::lookAt(cameraPos, -Z_AXIS, Y_AXIS);
	float lookingUpRad = 0, lookingSideRad = 0;

	glm::vec3 modelPos = glm::vec3(0, FLOOR_HEIGHT, 20);
	float modelLookingSideRad = 0;

	bool freecam = false;
	unsigned int numVerts;
	int VAO;
public:
	MainCharacter(unsigned int numVerts, GLuint VAO) : numVerts(numVerts), VAO(VAO) {}
	void flipFreecam();
	void update(GLFWwindow* window, AstroHandler* astroHandler);
	void mouseMove(double x, double y);
	void draw(GLuint shader);
	glm::mat4 getView();
	glm::vec3 getCameraPos();
	glm::vec3 getModelPos();
};