#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

struct OBJ {
	std::vector<GLfloat> mesh;
	std::vector<GLfloat> normals;
};

// Lots of constants to make random points at the beginning
constexpr auto m_x = -9;
constexpr auto m_y = -7;
constexpr auto m_z = m_x;
constexpr auto M_x = -m_x;
constexpr auto M_y = -m_y;
constexpr auto M_z = -m_z;
constexpr auto e_x = 1;
constexpr auto E_x = 1.5;
constexpr auto e_y = 5;
constexpr auto E_y = 10;

constexpr float mx = std::numeric_limits<float>::max(), mn = std::numeric_limits<float>::min();

OBJ loadObj(std::string filename, int windowWidth, int windowSize);
OBJ loadLightObj(std::string path, int width, int height);
glm::vec3 getCenters(std::vector<glm::vec3>& pc);
void getPointsAndNormals(std::string a, std::string b, std::string c, std::vector<unsigned int>& facesPoints, std::vector<unsigned int>& facesNormals);
std::vector<glm::vec3> centerAndResize(std::vector<glm::vec3> pc, int width, int height);
