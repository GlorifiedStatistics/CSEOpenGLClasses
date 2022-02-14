#include "Constants.h"
#include "Window.h"

void error_callback(int error, const char* description) {
	std::cerr << description << std::endl;
}

int main(void) {
	GLFWwindow* window = Window::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!window) exit(EXIT_FAILURE);

	// Setup callbacks.
	glfwSetErrorCallback(error_callback);
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
	glfwSetKeyCallback(window, Window::keyCallback);
	glfwSetCursorPosCallback(window, Window::mouseMoveCallback);
	glfwSetMouseButtonCallback(window, Window::mouseClickCallback);
	glfwSetScrollCallback(window, Window::scrollCallback);

	// Setup OpenGL settings.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Initialize everything
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);
	
	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window)) {
		Window::displayCallback(window);
		Window::idleCallback();
	}

	// Close everything
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}