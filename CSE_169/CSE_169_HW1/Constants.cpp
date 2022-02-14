#include "Constants.h"

std::vector<GLfloat> genMesh(glm::vec3 boxmin, glm::vec3 boxmax) {
	std::vector<GLfloat> ret = {
		boxmin.x, boxmin.y, boxmax.z,
		boxmin.x, boxmax.y, boxmax.z,
		boxmax.x, boxmax.y, boxmax.z,
		boxmax.x, boxmin.y, boxmax.z,

		boxmax.x, boxmin.y, boxmax.z,
		boxmax.x, boxmax.y, boxmax.z,
		boxmax.x, boxmax.y, boxmin.z,
		boxmax.x, boxmin.y, boxmin.z,

		boxmin.x, boxmin.y, boxmin.z,
		boxmin.x, boxmax.y, boxmin.z,
		boxmax.x, boxmax.y, boxmin.z,
		boxmax.x, boxmin.y, boxmin.z,

		boxmin.x, boxmin.y, boxmax.z,
		boxmin.x, boxmax.y, boxmax.z,
		boxmin.x, boxmax.y, boxmin.z,
		boxmin.x, boxmin.y, boxmin.z,
	};
	return ret;
}

glm::vec3 constrain(glm::vec3 dof, glm::vec2 rotxlimit, glm::vec2 rotylimit, glm::vec2 rotzlimit) {
	dof.x = dof.x < rotxlimit[0] ? rotxlimit[0] : dof.x > rotxlimit[1] ? rotxlimit[1] : dof.x;
	dof.y = dof.y < rotylimit[0] ? rotylimit[0] : dof.y > rotylimit[1] ? rotylimit[1] : dof.y;
	dof.z = dof.z < rotzlimit[0] ? rotzlimit[0] : dof.z > rotzlimit[1] ? rotzlimit[1] : dof.z;
	return dof;
}