#include "MainCharacter.h"

glm::vec3 getWASDTranslation(GLFWwindow* window, bool freecam) {
	float x = 0, z = 0, y = 0;
	float mult = (freecam ? FREECAM_MOVEMENT_MULT : 1) * (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? SLOW_MULT : 1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		z -= mult * MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		z += mult * MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		x += mult * MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		x -= mult * MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		y += mult * MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		y -= mult * MOVEMENT_SPEED;
	}
	return glm::vec3(x, y, z);
}

void MainCharacter::update(GLFWwindow* window, AstroHandler* astroHandler) {
	glm::vec3 translate = getWASDTranslation(window, freecam);
	glm::vec3 updatePos = glm::vec3(glm::rotate(I, -lookingSideRad, Y_AXIS) * glm::vec4(translate, 1));

	if (!freecam) {
		updatePos = glm::vec3(updatePos.x, 0, updatePos.z);
		modelPos += updatePos;
		modelLookingSideRad = lookingSideRad + DEG_180;

		modelPos += collideScene(modelPos);

		// Collide with other characters
		modelPos += astroHandler->collide(modelPos);

		glm::vec3 rot = glm::vec3(glm::rotate(I, lookingSideRad, Y_AXIS) * glm::vec4(CAMERA_OFFSET, 1));
		rot.x = -rot.x;
		cameraPos = modelPos + rot;
	}
	else {
		cameraPos += updatePos;
	}

	if (cameraPos.y < CAMERA_MIN_HEIGHT) cameraPos.y = CAMERA_MIN_HEIGHT;

	//if (translate.x != 0 || translate.y != 0 || translate.z != 0) {
	//	std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
	//}	

	glm::vec3 lookDir = glm::vec3(glm::rotate(I, -lookingSideRad, Y_AXIS) * glm::vec4(-Z_AXIS, 1));
	lookDir = glm::vec3(glm::rotate(I, -lookingUpRad, glm::cross(lookDir, Y_AXIS)) * glm::vec4(lookDir, 1));
	view = glm::lookAt(cameraPos, cameraPos + lookDir, Y_AXIS);
}

void MainCharacter::mouseMove(double x, double y) {
	lookingSideRad += (float)x * MOUSE_ROTATE_SPEED;
	lookingUpRad += (float)y * MOUSE_ROTATE_SPEED;

	if (lookingSideRad > DEG_360) lookingSideRad -= DEG_360;
	if (lookingSideRad < -DEG_360) lookingSideRad += DEG_360;

	float div = 2.0f + LOOKDOWN_OFFSET;

	if (lookingUpRad > pi / div) lookingUpRad = pi / div;
	if (lookingUpRad < -pi / div) lookingUpRad = -pi / div;

}

void MainCharacter::flipFreecam() {
	if (freecam) {
		lookingSideRad = modelLookingSideRad + DEG_180;
		lookingUpRad = 0;
	}
	freecam = !freecam;
}

void MainCharacter::draw(GLuint shader) {
	if (freecam) {
		glm::mat4 modelMat = glm::translate(modelPos) * glm::rotate(I, -modelLookingSideRad, Y_AXIS);

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(A_BLUE.color));

		// Bind the VAO
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
	}
}

glm::mat4 MainCharacter::getView() { return view; }
glm::vec3 MainCharacter::getCameraPos() { return cameraPos; }
glm::vec3 MainCharacter::getModelPos() { return modelPos; }