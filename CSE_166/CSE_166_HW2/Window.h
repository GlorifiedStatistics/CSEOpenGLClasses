#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "TriangleMesh.h"

#define _ROT_SCALE 4
#define _SCROLL_SCALE 1.1
#define ROTATE_MODEL 0
#define ROTATE_LIGHT 1
#define ROTATE_ALL 2

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	static glm::vec3 lastPoint, lastLightPoint;
	static bool clicked;
	static float currX;
	static float currY;

	// Objects to Render
	static TriangleMesh* objs[3];

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view, initialModelMat, initialLightModelMat;
	static glm::vec3 eyePos, lookAtPoint, upVector;
	static int rotateMode, normalMode;

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
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(float x, float y);
};

#endif
