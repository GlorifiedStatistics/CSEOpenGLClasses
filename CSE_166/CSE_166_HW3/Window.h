#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Geometry.h"
#include "Transform.h"
#include "Skybox.h"
#include "Floor.h"
#include "BuildScene.h"


class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static GLFWwindow* thisWindow;
	static const char* windowTitle;

	static float lookingUpRad, lookingSideRad;

	// Objects to Render
	static Transform* scene;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view, worldRotation, worldTranslation;
	static glm::vec3 cameraPos;
	static int cubemapTexture;

	// Shader Program ID
	static GLuint shaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static glm::vec3 getWASDTranslation(GLFWwindow* window);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double x, double y);
};

#endif
