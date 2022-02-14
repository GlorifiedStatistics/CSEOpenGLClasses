#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Constants.h"
#include "OBJ.h"

OBJ loadObj(std::string filename);
OBJ genDiscoSphere();
OBJ genFloor();
OBJ genSkybox();
OBJ genTriangle();