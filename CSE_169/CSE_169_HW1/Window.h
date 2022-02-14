#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "shader.h"
#include "Constants.h"
#include "LoadSkel.h"

class Window {
public:

	// Window Properties
	static int width, height;
	static const char* windowTitle;

	static glm::vec3 lastPoint, lastLightPoint;
	static bool clicked;
	static float currX, currY, scale;

	// Objects to Render
	static std::vector<Skeleton*> skeletons;
	static Skeleton* currModel;

	// Camera Matrices
	static glm::mat4 projection, view, initialModelMat;

	// Shader Program ID
	static GLuint shaderProgram, VAO;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();

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
