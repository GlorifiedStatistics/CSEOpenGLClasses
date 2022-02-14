#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
GLFWwindow* Window::thisWindow;
const char* Window::windowTitle = "Homework 4";


GLuint Window::VAOAstro, Window::VAOLobby, Window::VAOSphere;
unsigned int Window::astroSize, Window::lobbySize, Window::sphereSize;

// Objects to Render
MainCharacter* Window::mainCharacter;
AstroHandler* Window::astroHandler;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

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

void genBuffers(GLuint* VAO, unsigned int* outSize, OBJ model) {
	GLuint VBO, NB;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(*VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(GLfloat), model.getPoints().data(), GL_STATIC_DRAW);

	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


	// Set up the normal buffers
	glGenBuffers(1, &NB);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(GLfloat), model.getNormals().data(), GL_STATIC_DRAW);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	*outSize = model.size();
}

bool Window::initializeObjects(){
	genBuffers(&VAOAstro, &astroSize, loadObj(CHARACTER_OBJ_PATH));
	genBuffers(&VAOLobby, &lobbySize, loadObj(LOBBY_OBJ_PATH));
	genBuffers(&VAOSphere, &sphereSize, loadObj(SPHERE_OBJ_PATH));

	mainCharacter = new MainCharacter(astroSize, VAOAstro);
	astroHandler = new AstroHandler(astroSize, VAOAstro);

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
	mainCharacter->update(thisWindow, astroHandler);
	astroHandler->update(mainCharacter->getModelPos());
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Draw the scene
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, false, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(mainCharacter->getCameraPos()));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, false, glm::value_ptr(mainCharacter->getView()));

	// Set the lighting info
	glUniform3fv(glGetUniformLocation(shaderProgram, "light.pos"), 1, glm::value_ptr(MAIN_LIGHT.pos));
	glUniform3fv(glGetUniformLocation(shaderProgram, "light.intensities"), 1, glm::value_ptr(MAIN_LIGHT.intensities));
	glUniform1f(glGetUniformLocation(shaderProgram, "light.attenuation"), MAIN_LIGHT.attenuation);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.ambientCoefficient"), MAIN_LIGHT.ambientCoefficient);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), MAIN_LIGHT.exponent);

	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_COLOR"), DRAW_COLOR);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_NORMALS"), DRAW_NORMALS);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_PHONG"), DRAW_PHONG);
	glUniform1i(glGetUniformLocation(shaderProgram, "DRAW_TOON"), DRAW_TOON);

	glUniform1f(glGetUniformLocation(shaderProgram, "TOON_OUTLINE"), TOON_OUTLINE);

	glUniform3fv(glGetUniformLocation(shaderProgram, "matProps.ambient"), 1, glm::value_ptr(METAL.ambient));
	glUniform3fv(glGetUniformLocation(shaderProgram, "matProps.diffuse"), 1, glm::value_ptr(METAL.diffuse));
	glUniform3fv(glGetUniformLocation(shaderProgram, "matProps.specular"), 1, glm::value_ptr(METAL.specular));
	glUniform1f(glGetUniformLocation(shaderProgram, "matProps.shininess"), METAL.shininess);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Draw first the lobby and the light ball
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(I));
	glUniform1i(glGetUniformLocation(shaderProgram, "mode"), DRAW_PHONG);

	glBindVertexArray(VAOLobby);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, lobbySize);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(LIGHT_POS_MAT));
	glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(LIGHT_BALL_COLOR));
	glUniform1i(glGetUniformLocation(shaderProgram, "mode"), DRAW_COLOR);

	glBindVertexArray(VAOSphere);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, sphereSize);

	// Draw the characters
	glUniform1i(glGetUniformLocation(shaderProgram, "mode"), DRAW_TOON);

	mainCharacter->draw(shaderProgram);
	astroHandler->draw(shaderProgram);

	glUseProgram(0);
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_C:
			mainCharacter->flipFreecam();
			break;

		default:
			break;
		}
	}
}

void Window::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	mainCharacter->mouseMove(x, y);
	glfwSetCursorPos(window, 0, 0);
}