#pragma once

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>


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
const float RTOT = glm::sqrt(2.0f) / 2.0f;

constexpr glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
constexpr glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
constexpr glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);
constexpr glm::mat4 I = glm::mat4(1);


///////////////////////
// WINDOW.CPP STUFF //
//////////////////////
const std::string CHARACTER_OBJ_PATH = "assets/astro_still.obj";
const std::string SPHERE_OBJ_PATH = "assets/sphere.obj";
const std::string LOBBY_OBJ_PATH = "assets/lobby.obj";

constexpr float MOVEMENT_SPEED = 0.005f;
constexpr float FREECAM_MOVEMENT_MULT = 2.3f;
constexpr float MOUSE_ROTATE_SPEED = 5 * pi / 180.0f / 30;
constexpr float CAMERA_MIN_HEIGHT = -100.0f;
constexpr float SLOW_MULT = 0.1f;

constexpr unsigned int WINDOW_WIDTH = 1900;
constexpr unsigned int WINDOW_HEIGHT = 1200;

constexpr glm::vec3 CAMERA_OFFSET = glm::vec3(0, 2.5, 0.6);

constexpr float FLOOR_HEIGHT = 0.3f;
constexpr float MAX_Z = 21.19f;
constexpr float MIN_Z = -12.4f;
constexpr float MAX_X = 16.4f;
constexpr float MIN_X = -MAX_X;
constexpr glm::vec3 STEP_1 = glm::vec3(0.0f, 0.817f, 4.325f);
constexpr glm::vec3 STEP_2 = glm::vec3(0.0f, 1.312f, 3.756f);
constexpr glm::vec3 STEP_3 = glm::vec3(0.0f, 1.722f, 3.174f);

constexpr float LOOKDOWN_OFFSET = 0.01f;
constexpr float COLLISION_DIST = 1.2f;

struct Plane {
	glm::vec4 eq;
	glm::vec3 n;
	Plane(glm::vec3 tl, glm::vec3 br) {
		glm::vec3 bl = glm::vec3(tl.x, br.y, tl.z);
		glm::vec3 v1 = tl - br;
		glm::vec3 v2 = tl - bl;
		glm::vec3 c = glm::cross(v1, v2);
		float d = c.x * tl.x + c.y * tl.y + c.z * tl.z;
		eq = glm::vec4(c, -d);
		n = glm::normalize(c);
	}

	glm::vec3 collide(glm::vec3 p) {
		float dist = (eq.x * p.x + eq.y * p.y + eq.z * p.z + eq[3]) / glm::length(glm::vec3(eq));
		return glm::abs(dist) < COLLISION_DIST ? n * glm::sign(dist) * (COLLISION_DIST - glm::abs(dist)) : glm::vec3(0);
	}
};

const Plane CORNER_1(glm::vec3(-11.68f, 5.29f, 21.5f), glm::vec3(-16.78f, FLOOR_HEIGHT, 16.56f));
const Plane CORNER_2(glm::vec3(-16.5f, 6.33f, -0.18f), glm::vec3(-3.73f, FLOOR_HEIGHT, -12.42f));
const Plane CORNER_3(glm::vec3(3.75f, 6.33f, -12.63f), glm::vec3(16.35f, FLOOR_HEIGHT, 0.07f));
const Plane CORNER_4(glm::vec3(16.5f, 6.33f, 16.29f), glm::vec3(11.48f, FLOOR_HEIGHT, 21.27f));

const std::vector<Plane> COLLISION_PLANES = {CORNER_1, CORNER_2, CORNER_3, CORNER_4};

struct Box {
	glm::vec3 center;
	float r;
	Box(glm::vec3 p1, glm::vec3 p2) {
		glm::vec3 d = (p2 - p1) / 2.0f;
		center = p1 + d;
		r = glm::length(d) * RTOT;
	}

	glm::vec3 collide(glm::vec3 p) {
		p.y = 0;
		glm::vec3 d = center - p;
		float dist = length(d);
		return dist < COLLISION_DIST + r ? -(COLLISION_DIST + r - dist) * d : glm::vec3(0);
	}
};

const Box BOX_1(glm::vec3(-6.83f, 0.0f, 12.01f), glm::vec3(-12.59f, 0.0f, 10.77f));
const Box BOX_2(glm::vec3(7.66f, 0.0f, 8.57f), glm::vec3(13.55f, 0.0f, 7.65f));
const Box BOX_3(glm::vec3(-5.13f, 0.0f, 0.0f), glm::vec3(-10.06f, 0.0f, -1.05f));

const std::vector<Box> COLLISION_BOXES = { BOX_1, BOX_2, BOX_3 };


///////////////////////
// DRAWING CONSTANTS //
///////////////////////
constexpr unsigned int DRAW_NORMALS = 0;
constexpr unsigned int DRAW_COLOR = 1;
constexpr unsigned int DRAW_PHONG = 2;
constexpr unsigned int DRAW_TOON = 3;


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

const Material METAL = {
	glm::vec3(0.4f, 0.4f, 0.4f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	glm::vec3(0.9f, 0.9f, 0.9f),
	0.3f * 128.0f
};

const Light MAIN_LIGHT = {
	glm::vec3(13.5f, 11.0f, 18.5f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	0.01f,
	0.5f,
	1.5f
};

const glm::mat4 LIGHT_POS_MAT = glm::translate(MAIN_LIGHT.pos);
constexpr glm::vec3 LIGHT_BALL_COLOR = glm::vec3(0.9f, 0.9f, 0.9f);


/////////////////
// SCENE STUFF //
/////////////////

struct AstroColor {
	glm::vec3 color;
	AstroColor(std::string hex) {
		float r = (float)std::stoul(hex.substr(0, 2), NULL, 16) / 255.0f;
		float g = (float)std::stoul(hex.substr(2, 2), NULL, 16) / 255.0f;
		float b = (float)std::stoul(hex.substr(4, 2), NULL, 16) / 255.0f;
		color = glm::vec3(r, g, b);
	}
};

const glm::vec4 SPAWN_BOX(-11.5f, 9.26f, -3.96f, 20.22f);

const AstroColor A_BLACK("3E474E");
const AstroColor A_BLUE("132ED1");
const AstroColor A_BROWN("71491D");
const AstroColor A_CYAN("39FEDD");
const AstroColor A_GREEN("13802C");
const AstroColor A_LIME("4EEF38");
const AstroColor A_ORANGE("F17D0C");
const AstroColor A_PINK("EC54BB");
const AstroColor A_PURPLE("6C2FBC");
const AstroColor A_RED("C51211");
const AstroColor A_WHITE("D6DFF1");
const AstroColor A_YELLOW("F6F657");

const std::vector<AstroColor> ASTRO_COLORS = {
	A_BLACK, A_BROWN, A_CYAN, A_GREEN, A_LIME,
	A_ORANGE, A_PINK, A_PURPLE, A_RED, A_WHITE, A_YELLOW
};

constexpr unsigned int NUM_ASTROS = 11;
constexpr unsigned int MAX_ASTROS = 9;

constexpr float TOON_OUTLINE = 0.3f;

constexpr long MIN_SPAWN_WAIT_TIME = 4 * 1000;
constexpr long MAX_SPAWN_WAIT_TIME = 5 * 1000;
constexpr long MIN_EXIST_TIME = 2 * 1000;
constexpr long MAX_EXIST_TIME = 60 * 1000;
constexpr long MIN_WAIT_TIME = 1 * 1000;
constexpr long MAX_WAIT_TIME = 5 * 1000;

constexpr float RANDOM_STOP_PERCENT = 0.0001f;
