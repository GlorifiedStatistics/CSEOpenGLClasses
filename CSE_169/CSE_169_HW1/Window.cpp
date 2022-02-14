#include "Window.h"

// Window Properties
int Window::width, Window::height;
const char* Window::windowTitle = "Homework 1";

// Camera movement
glm::vec3 Window::lastPoint, Window::lastLightPoint;
bool Window::clicked;
float Window::currX, Window::currY, Window::scale = 1.0f;

// Objects to Render
std::vector<Skeleton*> Window::skeletons;
Skeleton* Window::currModel;

// View and projection things
glm::mat4 Window::view = glm::lookAt(CAMERA_START, CAMERA_START - Z_AXIS, Y_AXIS);
glm::mat4 Window::projection, Window::initialModelMat;

// Shader Program ID
GLuint Window::shaderProgram; 


bool Window::initializeProgram() {
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	if (!shaderProgram) {
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects() {
	skeletons.push_back(loadSkel("assets/test.skel"));
	skeletons.push_back(loadSkel("assets/wasp.skel"));
	skeletons.push_back(loadSkel("assets/dragon.skel"));
	currModel = skeletons[0];

	return true;
}

GLFWwindow* Window::createWindow(int width, int height) {
	// Initialize GLFW.
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing.

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (!window) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

	if (glewInit()) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);
	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
	Window::width = width; Window::height = height;
	glViewport(0, 0, width, height);
	Window::projection = glm::perspective(glm::radians(60.0), double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback() {
	// Update things
}

void Window::displayCallback(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, false, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(JOINT_COLOR));

	// Render the objects
	currModel->draw(shaderProgram);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);				
				break;

			case GLFW_KEY_R:
				currModel->M = glm::mat4(1);
				scale = 1.0f;
				break;

			case GLFW_KEY_1:
				currModel = skeletons[0];
				currModel->M = glm::mat4(1);
				scale = 1.0f;
				break;
			case GLFW_KEY_2:
				currModel = skeletons[1];
				currModel->M = glm::mat4(1);
				scale = 1.0f;
				break;
			case GLFW_KEY_3:
				currModel = skeletons[2];
				currModel->M = glm::mat4(1);
				scale = 1.0f;
				break;

			default:
				break;
		}
	}
}

glm::vec3 Window::trackBallMapping(float x, float y) {
	glm::vec3 v( (x - width/2) / width, (height/2 - y) / height, 0 );
	float d = glm::length(v);
	d = (d > 1.0f) ? 1.0f : d;
	v.z = sqrtf(1.0f - d * d);
	return glm::normalize(v);
}

glm::vec3 orientPoint(glm::vec3 point, glm::mat4 model) {
	glm::vec4 op = glm::inverse(model) * glm::vec4(point, 1.0f);
	return glm::vec3(op.x, op.y, op.z);
}

void Window::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	currX = (float)x; currY = (float)y;
	if (clicked) {
		glm::vec3 currPoint = orientPoint(trackBallMapping((float)x, (float)y), initialModelMat);
		glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
		float rotAngle = glm::length(currPoint - lastPoint) * ROT_SCALE * scale;

		if (rotAxis.x != 0 || rotAxis.y != 0 || rotAxis.z != 0) currModel->M = glm::rotate(initialModelMat, rotAngle, rotAxis);
	}
}

void Window::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		initialModelMat = currModel->M;
		lastPoint = orientPoint(trackBallMapping(currX, currY), initialModelMat);
		clicked = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		clicked = false;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	float scale = yoffset > 0 ? yoffset * SCROLL_SCALE : -1 / (yoffset * SCROLL_SCALE);
	Window::scale = Window::scale * scale;
	currModel->M = glm::scale(currModel->M, glm::vec3(scale, scale, scale));
}