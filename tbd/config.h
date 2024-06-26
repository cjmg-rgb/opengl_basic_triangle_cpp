#pragma once


#include "iostream"
#include "fstream"
#include "string"

#include "glew.h"
#include "glfw3.h"
#include "SOIL2.h"

#include "glm.hpp"
#include "mat4x4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

Vertex vertices[] =
{
	glm::vec3(-.5f, .5f, .0f), glm::vec3(1.f, .0f, .0f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(-.5f,-.5f, .0f), glm::vec3(0.f, 1.f, .0f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(.5f,-.5f, .0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(.5f, .5f, .0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, -1.f),
};

unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3
};