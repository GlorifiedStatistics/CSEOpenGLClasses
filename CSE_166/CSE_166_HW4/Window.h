#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "AstroHandler.h"
#include "MainCharacter.h"
#include "LoadObj.h"


class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static GLFWwindow* thisWindow;
	static const char* windowTitle;

	static GLuint VAOAstro, VAOLobby, VAOSphere;
	static unsigned int astroSize, lobbySize, sphereSize;

	static MainCharacter* mainCharacter;
	static AstroHandler* astroHandler;

	// Camera Matrices
	static glm::mat4 projection;

	// Shader Program ID
	static GLuint shaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double x, double y);
};

#endif
