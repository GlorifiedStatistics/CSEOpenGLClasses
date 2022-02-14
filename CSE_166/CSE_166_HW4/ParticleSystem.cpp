#include "ParticleSystem.h"

long currentTimePart() {
	return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
}

void ParticleSystem::init(glm::vec3 pos) {
	this->pos = pos;
	this->initTime = currentTimePart();
}

void ParticleSystem::draw() {
	if (!drawable) return;
}