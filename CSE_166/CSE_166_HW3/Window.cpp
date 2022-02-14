#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
GLFWwindow* Window::thisWindow;
const char* Window::windowTitle = "Homework 3";

int Window::cubemapTexture;

// Objects to Render
Transform* Window::scene;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::cameraPos(0, CAMERA_MIN_HEIGHT, 20);			// Camera position.
glm::mat4 Window::view = glm::lookAt(cameraPos, -Z_AXIS, Y_AXIS);
float Window::lookingUpRad = 0;
float Window::lookingSideRad = 0;

// Shader Program ID
GLuint Window::shaderProgram; 


bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects(){
	scene = new Transform();

	Transform* floor = new Transform();
	Geometry* floorGeom = new Geometry(genFloor());
	floor->translate(FLOOR_TRANSLATE)->addChild(floorGeom->set(DRAW_FLOOR));

	scene->addChild(buildScene())
		 ->addChild(floor)
		 ->addChild(new Skybox(SKYBOX_FACES_PATHS));

	return true;
}

void Window::cleanUp() {
	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	glfwSetCursorPos(window, 0, 0);

	thisWindow = window;

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}


void Window::idleCallback() {
	//worldRotation = glm::rotate(glm::mat4(1), lookingSideRad, glm::vec3(0, 1, 0));
	//worldRotation = glm::rotate(glm::mat4(1), lookingUpRad, glm::vec3(glm::rotate(glm::mat4(1), -lookingSideRad, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1))) * worldRotation;

	cameraPos += glm::vec3(glm::rotate(I, -lookingSideRad, Y_AXIS) * glm::vec4(getWASDTranslation(thisWindow), 1));
	if (COLLIDE_FLOOR && cameraPos.y < CAMERA_MIN_HEIGHT) cameraPos.y = CAMERA_MIN_HEIGHT;

	glm::vec3 lookDir = glm::vec3(glm::rotate(I, -lookingSideRad, Y_AXIS) * glm::vec4(-Z_AXIS, 1));
	lookDir = glm::vec3(glm::rotate(I, -lookingUpRad, glm::cross(lookDir, Y_AXIS)) * glm::vec4(lookDir, 1));
	view = glm::lookAt(cameraPos, cameraPos + lookDir, Y_AXIS);

	scene->update();
}

glm::vec3 Window::getWASDTranslation(GLFWwindow* window) {
	float x = 0, z = 0, y = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		z -= MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		z += MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		x += MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		x -= MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		y += MOVEMENT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		y -= MOVEMENT_SPEED;
	}
	return glm::vec3(x, y, z);
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Draw the scene
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, false, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(cameraPos));

	// Set the lighting info
	glUniform3fv(glGetUniformLocation(shaderProgram, "light.pos"), 1, glm::value_ptr(MAIN_LIGHT.pos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "light.intensities"), 1, glm::value_ptr(MAIN_LIGHT.intensities));
	glUniform1f(glGetUniformLocation(shaderProgram, "light.attenuation"), MAIN_LIGHT.attenuation);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.ambientCoefficient"), MAIN_LIGHT.ambientCoefficient);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), MAIN_LIGHT.exponent);

	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_COLOR"), DRAW_COLOR);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_CUBEMAP"), DRAW_CUBEMAP);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_NORMALS"), DRAW_NORMALS);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_PHONG"), DRAW_PHONG);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_REFLECTION"), DRAW_REFLECTION);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_FLOOR"), DRAW_FLOOR);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	scene->draw(glm::mat4(1), view, cameraPos, shaderProgram);

	glUseProgram(0);
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_1:
			toggleRot(1);
			break;
		case GLFW_KEY_2:
			toggleRot(2);
			break;
		case GLFW_KEY_3:
			toggleRot(3);
			break;

		default:
			break;
		}
	}
}

void Window::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	lookingSideRad += (float)x * MOUSE_ROTATE_SPEED;
	lookingUpRad += (float)y * MOUSE_ROTATE_SPEED;

	if (lookingSideRad > DEG_360) lookingSideRad -= DEG_360;
	if (lookingSideRad < -DEG_360) lookingSideRad += DEG_360;

	if (lookingUpRad > pi / 2.01f) lookingUpRad = pi / 2.01f;
	if (lookingUpRad < -pi / 2.01f) lookingUpRad = -pi / 2.01f;

	glfwSetCursorPos(window, 0, 0);
}