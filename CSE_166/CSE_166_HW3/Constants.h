#pragma once
#include <glm/glm.hpp>

////////////////////
// MATH CONSTANTS //
////////////////////
constexpr float pi = 3.141592f;
constexpr float DEG_90 = pi / 2.0f;
constexpr float DEG_180 = pi;
constexpr float DEG_270 = 3.0f * pi / 4.0f;
constexpr float DEG_360 = 2.0f * pi;
constexpr float DEG_45 = pi / 4.0f;
constexpr float DEG_120 = 2.0f * pi / 3.0f;
constexpr float DEG_240 = 4.0f * pi / 3.0f;

constexpr glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
constexpr glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
constexpr glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);
constexpr glm::mat4 I = glm::mat4(1);


///////////////////////
// WINDOW.CPP STUFF //
//////////////////////
const std::string SPHERE_PATH = "./assets/sphere.obj";
const std::string CYLINDER_PATH = "./assets/cylinder.obj";

const std::vector<std::string> SKYBOX_FACES_PATHS = {
	"./assets/skybox/right.jpg",
	"./assets/skybox/left.jpg",
	"./assets/skybox/top.jpg",
	"./assets/skybox/bottom.jpg",
	"./assets/skybox/front.jpg",
	"./assets/skybox/back.jpg"
};

constexpr float MOVEMENT_SPEED = 0.005f;
constexpr float MOUSE_ROTATE_SPEED = 5 * pi / 180.0f / 30;
constexpr float CAMERA_MIN_HEIGHT = 3.0f;
constexpr bool COLLIDE_FLOOR = true;

constexpr unsigned int WINDOW_WIDTH = 1900;
constexpr unsigned int WINDOW_HEIGHT = 1200;


//////////////////////
// FLOOR AND SKYBOX //
//////////////////////
constexpr float FLOOR_SIZE = 300;
constexpr float SKYBOX_SIZE = 512;
constexpr glm::vec3 FLOOR_TRANSLATE = glm::vec3(0, 1, 0);


///////////////////////
// DRAWING CONSTANTS //
///////////////////////
constexpr unsigned int DRAW_NORMALS = 0;
constexpr unsigned int DRAW_COLOR = 1;
constexpr unsigned int DRAW_REFLECTION = 2;
constexpr unsigned int DRAW_CUBEMAP = 3;
constexpr unsigned int DRAW_PHONG = 4;
constexpr unsigned int DRAW_FLOOR = 5;


/////////////
// STRUCTS //
/////////////
struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct Light {
	glm::vec3 pos;
	glm::vec3 intensities;
	GLfloat attenuation;
	GLfloat ambientCoefficient;
	GLfloat exponent;
};

struct RotateInfo {
	RotateInfo(const float angle, const glm::vec3 axis) : angle(angle), axis(axis) {}
	const float angle;
	const glm::vec3 axis;
};

const Material BRASS = {
	glm::vec3(0.329412f, 0.223529f, 0.027451f),
	glm::vec3(0.780392f, 0.568627f, 0.113725f),
	glm::vec3(0.992157f, 0.941176f, 0.807843f),
	0.3f * 128.0f
};

const Material METAL = {
	glm::vec3(0.4f, 0.4f, 0.4f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	glm::vec3(0.9f, 0.9f, 0.9f),
	0.3f * 128.0f
};

const Material BLACK_METAL = {
	glm::vec3(0.1f, 0.1f, 0.1f),
	glm::vec3(0.2f, 0.2f, 0.2f),
	glm::vec3(0.8f, 0.8f, 0.8f),
	0.6f * 128.0f
};

const Material RED_METAL = {
	glm::vec3(0.3f, 0.1f, 0.1f),
	glm::vec3(0.6f, 0.2f, 0.2f),
	glm::vec3(0.8f, 0.1f, 0.1f),
	0.2f * 128.0f
};

const Material BLUE_METAL = {
	glm::vec3(0.3f, 0.1f, 0.1f),
	glm::vec3(0.2f, 0.2f, 0.6f),
	glm::vec3(0.8f, 0.1f, 0.1f),
	0.2f * 128.0f
};

const Material GREEN_METAL = {
	glm::vec3(0.3f, 0.1f, 0.1f),
	glm::vec3(0.2f, 0.6f, 0.2f),
	glm::vec3(0.8f, 0.1f, 0.1f),
	0.2f * 128.0f
};

const Light MAIN_LIGHT = {
	glm::vec3(-10.0f, 10.0f, -20.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	0.01f,
	0.5f,
	1.0f
};


////////////////////
// SCENE BUILDING //
////////////////////


// The disco ball
constexpr unsigned int DISCO_SPHERE_STACK_COUNT = 40;
constexpr unsigned int DISCO_SPHERE_SECTOR_COUNT = DISCO_SPHERE_STACK_COUNT;
constexpr float DISCO_BALL_SIZE = 2;
constexpr float DISCO_ROTATE_SPEED = 0.0001f;
constexpr glm::vec3 DISCO_TRANS = glm::vec3(-10, 0, -10);
constexpr glm::vec3 DISCO_POLE_STRETCH = glm::vec3(0.6, 8, 0.6);
constexpr glm::vec3 DISCO_ARM_STRETCH = glm::vec3(0.3, 6, 0.3);
const RotateInfo DISCO_ROTATE = RotateInfo(DEG_270, Y_AXIS);

// The ride
constexpr float _RIDE_BASE_SIZE = 5.0f;
constexpr float _RIDE_BASE_Y_STRETCH = 1.0f;
constexpr float _RIDE_BALL_SIZE_MULT = 1.2f;

constexpr float RIDE_BALL_ROTATE_SPEED = 0.0006f;
constexpr glm::vec3 RIDE_TRANSLATE = glm::vec3(30, 0, -20);
constexpr glm::vec3 RIDE_BASE_STRETCH = glm::vec3(_RIDE_BASE_SIZE, _RIDE_BASE_SIZE * _RIDE_BASE_Y_STRETCH, _RIDE_BASE_SIZE);
constexpr float RIDE_BALL_STRETCH = _RIDE_BASE_SIZE * _RIDE_BALL_SIZE_MULT;
constexpr glm::vec3 RIDE_BALL_TRANSLATE = glm::vec3(0, _RIDE_BASE_SIZE * (_RIDE_BALL_SIZE_MULT + _RIDE_BASE_Y_STRETCH) * 0.9f, 0);

// The ride arm
constexpr float _RIDE_ARM_SIZE = 15.0f;
constexpr float RIDE_ARM_ROTATION_SPEED = 0.0002f;
constexpr float RIDE_RIDE_ROTATION_SPEED = 0.0004f;
constexpr float RIDE_ARM_ROTATE_DEG = DEG_45 / 2.0f;
constexpr glm::vec3 RIDE_ARM_1_STRETCH = glm::vec3(1, _RIDE_ARM_SIZE, 1);
constexpr glm::vec3 RIDE_ARM_2_STRETCH = glm::vec3(1, _RIDE_ARM_SIZE * 2.0f / 3.0f, 1);
constexpr glm::vec3 RIDE_ARM_TRANSLATE = glm::vec3(0, 2.5, 4);
constexpr glm::vec3 RIDE_ARM_2_TRANSLATE = glm::vec3(0, 3.6, 17.7);
constexpr glm::vec3 RIDE_ARM_JOINT_1_TRANSLATE = glm::vec3(0, 10.7, 10.7);
constexpr glm::vec3 RIDE_ARM_JOINT_2_TRANSLATE = glm::vec3(0, 3, 18);
constexpr glm::vec3 RIDE_RIDE_ARM_STRETCH = glm::vec3(0.25, 8, 0.25);
constexpr glm::vec3 RIDE_RIDE_TRANSLATE = glm::vec3(0, -4, 7);
constexpr float RIDE_RIDE_SPHERE_SIZE = 1;
constexpr glm::vec3 RIDE_RIDE_SPHERE_TRANSLATE = glm::vec3(0, 0, 0);
constexpr glm::vec3 RIDE_RIDE_ROTATE_POINT = glm::vec3(0, 1, -7);
constexpr glm::vec3 RIDE_ARM_ROTATE_POINT = glm::vec3(0, 0, 0);