#include "Astro.h"

long currentTimeAstro() {
	return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
}

long randWaitAstro() {
	return rand() % (MAX_EXIST_TIME - MIN_EXIST_TIME) + MIN_EXIST_TIME;
}

long randWaitStandAstro() {
	return rand() % (MAX_WAIT_TIME - MIN_WAIT_TIME) + MIN_WAIT_TIME;
}

void Astro::draw(GLuint shader, AstroColor astroColor) {
	if (!drawable) return;

	glm::mat4 modelMat = glm::translate(pos) * glm::rotate(I, lookingRad, Y_AXIS) * I;

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(astroColor.color));

	// Bind the VAO
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
}


float randf(float min, float max) { return min + (float)rand() / ((float)(RAND_MAX / (max - min))); }

glm::vec3 randomPos() {
	return glm::vec3(randf(SPAWN_BOX[0], SPAWN_BOX[1]), 0, randf(SPAWN_BOX[2], SPAWN_BOX[3]));
}

glm::vec3 Astro::collideCharacters(std::vector<Astro*> astros, glm::vec3 mainCharacterPos, glm::vec3 point) {
	glm::vec3 ret = glm::vec3(0);
	point.y = 0;

	for (Astro* astro : astros) {
		if (astro->id == id) continue; 
		glm::vec3 d = astro->pos - point;
		float dist = length(d);
		ret += dist < 2 * COLLISION_DIST ? -(2 * COLLISION_DIST - dist) * d : glm::vec3(0);
	}

	glm::vec3 d = mainCharacterPos - point;
	float dist = length(d);
	return ret + (dist < 2 * COLLISION_DIST ? -(2 * COLLISION_DIST - dist) * d : glm::vec3(0));
}

void Astro::init(std::vector<Astro*> astros, glm::vec3 mainCharacterPos) {
	pos = randomPos();
	pos += collideScene(pos);
	pos += collideCharacters(astros, mainCharacterPos, pos);
	drawable = true;
	existTime = randWaitAstro();
	initTime = currentTimeAstro();
	lookingRad = randf(0, DEG_360);
}

void Astro::update(std::vector<Astro*> astros, glm::vec3 mainCharacterPos) {
	if (!drawable) return;

	if (currentTimeAstro() - initTime > existTime) {
		drawable = false;
	}

	glm::vec3 inc = glm::vec3(glm::rotate(I, lookingRad, Y_AXIS) * glm::vec4(0.0f, 0.0f, MOVEMENT_SPEED, 1.0f));
	if (waitInitTime < 0) {
		pos += inc;

		if (randf(0, 1.0f) < RANDOM_STOP_PERCENT) {
			waitInitTime = currentTimeAstro();
			waitTime = randWaitStandAstro();
		}
	}
	else if (currentTimeAstro() - waitInitTime > waitTime) {
		waitInitTime = -1;
		lookingRad = randf(0, DEG_360);
	}

	glm::vec3 diff = collideScene(pos) + collideCharacters(astros, mainCharacterPos, pos);
	pos += diff;
}

glm::vec3 Astro::collide(glm::vec3 point) {
	point.y = 0;
	glm::vec3 d = pos - point;
	float dist = length(d);
	return dist < 2 * COLLISION_DIST ? -(2 * COLLISION_DIST - dist) * d : glm::vec3(0);
}
