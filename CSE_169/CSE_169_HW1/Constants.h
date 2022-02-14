#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>

#define _R(RAD, AX) glm::rotate(glm::mat4(1), RAD, AX)

/////////////////
// MATH THINGS //
/////////////////

const glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
const glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
const glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);


//////////////////////
// Window.cpp Stuff //
//////////////////////

const float ROT_SCALE = 8.0f;
const float SCROLL_SCALE = 1.1f;

constexpr unsigned int WINDOW_WIDTH = 1900;
constexpr unsigned int WINDOW_HEIGHT = 1200;

const glm::vec3 CAMERA_START = glm::vec3(0, 0, 20);


////////////////////////
// Skeleton Constants //
////////////////////////

const glm::vec3 JOINT_COLOR = glm::vec3(1, 1, 1);

const glm::vec3 DEFAULT_OFFSET = glm::vec3(0);
const glm::vec3 DEFAULT_POSE = glm::vec3(0);
const glm::vec3 DEFAULT_BOXMIN = glm::vec3(-0.1f, -0.1f, -0.1f);
const glm::vec3 DEFAULT_BOXMAX = glm::vec3(0.1f, 0.1f, 0.1f);
const glm::vec2 DEFAULT_ROTLIM = glm::vec2(-100000.0f, 100000.0f);

///////////////////////
// Utility Functions //
///////////////////////

std::vector<GLfloat> genMesh(glm::vec3 boxmin, glm::vec3 boxmax);
glm::vec3 constrain(glm::vec3 dof, glm::vec2 rotxlimit, glm::vec2 rotylimit, glm::vec2 rotzlimit);