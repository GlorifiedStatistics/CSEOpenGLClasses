#include "AstroHandler.h"

long currentTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
}

long randWait() {
	return rand() % (MAX_SPAWN_WAIT_TIME - MIN_SPAWN_WAIT_TIME) + MIN_SPAWN_WAIT_TIME;
}

AstroHandler::AstroHandler(unsigned int numVerts, GLuint VAO) {
	for (unsigned int i = 0; i < NUM_ASTROS; i++) {
		astros.push_back(new Astro(numVerts, VAO, i));
	}
	srand((int)currentTime);

	lastIncommingCharacter = currentTime();
	randomWaitTime = randWait();
}

void AstroHandler::draw(GLuint shader) {
	for (unsigned int i = 0; i < NUM_ASTROS; i++) astros[i]->draw(shader, ASTRO_COLORS[i]);
}

void initRandomAstro(std::vector<Astro*> astros, glm::vec3 mainCharacterPos) {
	unsigned int numAvailable = 0;
	for (Astro* astro : astros) if (!astro->drawable) numAvailable++;

	int r = rand() % numAvailable;

	for (Astro* astro : astros) {
		if (astro->drawable) continue;
		if (r == 0) {
			astro->init(astros, mainCharacterPos);
			break;
		}
		r--;
	}
}

void AstroHandler::update(glm::vec3 mainCharacterPos) {
	unsigned int count = 0;
	for (unsigned int i = 0; i < NUM_ASTROS; i++) if (astros[i]->drawable) count++;

	if (count < MAX_ASTROS && (currentTime() - lastIncommingCharacter >= randomWaitTime)) {
		initRandomAstro(astros, mainCharacterPos);
		lastIncommingCharacter = currentTime();
		randomWaitTime = randWait();
	}

	for (unsigned int i = 0; i < NUM_ASTROS; i++) astros[i]->update(astros, mainCharacterPos);
}

glm::vec3 AstroHandler::collide(glm::vec3 point) {
	glm::vec3 ret = glm::vec3(0);
	for (Astro* astro : astros) if (astro->drawable) ret += astro->collide(point);
	return ret;
}

AstroHandler::~AstroHandler() {
	for (unsigned int i = 0; i < NUM_ASTROS; i++) {
		free(astros[i]);
	}
}
