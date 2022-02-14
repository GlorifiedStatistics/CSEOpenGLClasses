#include "Collision.h"

glm::vec3 collideScene(glm::vec3 point) {
	glm::vec3 ret = glm::vec3(0);

	// Collide with cardinal walls
	if (point.z + COLLISION_DIST > MAX_Z) {
		ret.z = MAX_Z - COLLISION_DIST - point.z;
	}
	else if (point.z - COLLISION_DIST < MIN_Z){
		ret.z = MIN_Z + COLLISION_DIST - point.z;
	}
	if (point.x - COLLISION_DIST < MIN_X) {
		ret.x = MIN_X + COLLISION_DIST - point.x;
	}
	else if (point.x + COLLISION_DIST > MAX_X) {
		ret.x = MAX_X - COLLISION_DIST - point.x;
	}

	// Collide with diagonal walls
	for (Plane plane : COLLISION_PLANES) {
		glm::vec3 c = plane.collide(point);
		ret += c; 
	}

	// Collide with boxes
	for (Box box : COLLISION_BOXES) {
		glm::vec3 c = box.collide(point);
		ret += c;
	}

	// Handle y value
	if (point.z > STEP_1.z) {
		ret.y = FLOOR_HEIGHT - point.y;
	}
	else if (point.z > STEP_2.z) {
		ret.y = STEP_1.y - point.y;
	}
	else if (point.z > STEP_3.z) {
		ret.y = STEP_2.y - point.y;
	}
	else {
		ret.y = STEP_3.y - point.y;
	}

	return ret;
}
