#pragma once
#include "Constants.h"

class ParticleSystem {
private:
	glm::vec3 pos;
	bool drawable;
	long initTime;

	glm::vec3 particles;
public:
	void init(glm::vec3 pos);
	void draw();
};