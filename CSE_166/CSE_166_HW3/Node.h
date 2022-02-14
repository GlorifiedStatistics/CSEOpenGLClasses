#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "LoadObj.h"
#include "Constants.h"

class Node {
public:
	virtual void update() = 0;
	virtual void draw(glm::mat4 C, glm::mat4 view, glm::vec3 cameraPos, GLuint shader) = 0;
};