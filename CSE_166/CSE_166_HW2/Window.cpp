#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

glm::vec3 Window::lastPoint;
glm::vec3 Window::lastLightPoint;
bool Window::clicked;
float Window::currX;
float Window::currY;
int Window::rotateMode = ROTATE_MODEL;
int Window::normalMode = 0;

// Objects to Render
TriangleMesh* Window::objs[3];
int currObj = 0;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
glm::mat4 Window::initialModelMat;
glm::mat4 Window::initialLightModelMat;

// Shader Program ID
GLuint Window::shaderProgram; 

// Increment point size
GLfloat pointInc = 1;



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

	// Create a point cloud consisting of cube vertices.
	std::vector<std::string> filePaths = {"assets/bunny.obj", "assets/sandal.obj", "assets/bear.obj"};

	// Make the light
	glm::vec3 lightPos = glm::vec3(0, 5, 15);
	OBJ lightObj = loadLightObj("assets/sphere.obj", width, height);

	for (unsigned int i = 0; i < lightObj.mesh.size(); i+=3) {
		lightObj.mesh[i] += lightPos.x;
		lightObj.mesh[i + 1] += lightPos.y;
		lightObj.mesh[i + 2] += lightPos.z;
	}

	Light light = {
		lightObj, 
		lightPos,					// Position
		glm::vec3(0.9, 0.2, 0.9),	// Light intensities
		0.05,						// attenuation
		1,							// ambientCoefficient
		1.15						// Exponential dropoff
	};

	objs[0] = new TriangleMesh(filePaths[0], emeraldProperties, light, &normalMode, width, height);
	cout << "Loaded 1" << endl;
	objs[1] = new TriangleMesh(filePaths[1], brassProperties, light, &normalMode, width, height);
	cout << "Loaded 2" << endl;
	objs[2] = new TriangleMesh(filePaths[2], bronzeProperties, light, &normalMode, width, height);
	cout << "Loaded 3" << endl;

	return true;
}

void Window::cleanUp()
{
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

void Window::idleCallback()
{
	// Perform any necessary updates here 
	objs[currObj]->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects
	objs[currObj]->draw(view, projection, shaderProgram);

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

		// switch between the cube and the cube pointCloud
		case GLFW_KEY_1:
			currObj = 0;
			break;
		case GLFW_KEY_2:
			currObj = 1;
			break;
		case GLFW_KEY_3:
			currObj = 2;
			break;
		case GLFW_KEY_Z:
			rotateMode = ROTATE_MODEL;
			break;
		case GLFW_KEY_X:
			rotateMode = ROTATE_LIGHT;
			break;
		case GLFW_KEY_C:
			rotateMode = ROTATE_ALL;
			break;
		case GLFW_KEY_N:
			normalMode = normalMode == 0 ? 1 : 0;
			break;

		default:
			break;
		}
	}
}

glm::vec3 Window::trackBallMapping(float x, float y) {
	glm::vec3 v( (x - width/2) / width, (height/2 - y) / height, 0 );
	float d = glm::length(v);
	d = (d > 1.0) ? 1.0 : d;
	v.z = sqrtf(1.0 - d*d);
	return glm::normalize(v);
}

glm::vec3 orientPoint(glm::vec3 point, glm::mat4 model) {
	glm::vec4 op = glm::inverse(model) * glm::vec4(point, 1.0f);
	return glm::vec3(op.x, op.y, op.z);
}

void Window::mouseMoveCallback(GLFWwindow* window, double x, double y) {
	currX = x;
	currY = y;
	if (clicked) {
		if (rotateMode == ROTATE_MODEL || rotateMode == ROTATE_ALL) {
			glm::vec3 currPoint = orientPoint(trackBallMapping((float)x, (float)y), initialModelMat);
			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			float rotAngle = glm::length(currPoint - lastPoint) * _ROT_SCALE;

			if (rotAxis.x != 0 || rotAxis.y != 0 || rotAxis.z != 0)
				objs[currObj]->model = glm::rotate(initialModelMat, rotAngle, rotAxis);
		}
		
		if (rotateMode == ROTATE_LIGHT || rotateMode == ROTATE_ALL) {
			glm::vec3 currPoint = orientPoint(trackBallMapping((float)x, (float)y), initialLightModelMat);
			glm::vec3 rotAxis = glm::cross(lastLightPoint, currPoint);
			float rotAngle = glm::length(currPoint - lastLightPoint) * _ROT_SCALE;

			if (rotAxis.x != 0 || rotAxis.y != 0 || rotAxis.z != 0)
				objs[currObj]->lightModel = glm::rotate(initialLightModelMat, rotAngle, rotAxis);

		}
	}
}

void Window::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		initialModelMat = objs[currObj]->model;
		initialLightModelMat = objs[currObj]->lightModel;
		lastPoint = orientPoint(trackBallMapping(currX, currY), initialModelMat);
		lastLightPoint = orientPoint(trackBallMapping(currX, currY), initialLightModelMat);
		clicked = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		clicked = false;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	float scale = yoffset > 0 ? yoffset * _SCROLL_SCALE : -1 / (yoffset * _SCROLL_SCALE);

	if (rotateMode == ROTATE_MODEL || rotateMode == ROTATE_ALL) {
		objs[currObj]->scaleModel(scale);
	}

	if (rotateMode == ROTATE_LIGHT || rotateMode == ROTATE_ALL) {
		if (rotateMode != ROTATE_ALL) scale = 1.0 / scale;
		objs[currObj]->scaleLight(scale);
	}
}