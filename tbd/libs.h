#pragma once

#include "iostream"
#include "vector"
#include "string"
#include "fstream"

#include "glew.h"
#include "glfw3.h"
#include "SOIL2.h"
#include "glm.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat4x4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

float vertices[] =
{
   -.9f, .5f, .0f, 1.f, 0.f, 0.f, 0.f, 1.f, // UP-LEFT
   -.9f,-.5f, .0f, 0.f, 0.f, 1.f, 0.f, 0.f, // BOT-LEFT
   -.1f,-.5f, .0f, 1.f, 0.f, 0.f, 1.f, 0.f, // BOT-RIGHT
   -.1f, .5f, .0f, 0.f, 0.f, 1.f, 1.f, 1.f // UP-RIGHT
};

float vertices2[] =
{
   .9f, .5f, .0f, 1.f, 0.f, 1.f, 1.f, 1.f, // UP-LEFT
   .9f,-.5f, .0f, 0.f, 0.f, 1.f, 1.f, 0.f, // BOT-LEFT
   .1f,-.5f, .0f, 1.f, 0.f, 1.f, 0.f, 0.f, // BOT-RIGHT
   .1f, .5f, .0f, 0.f, 0.f, 0.f, 0.f, 1.f // UP-RIGHT
};

int indices[] =
{
	0, 1, 2,
	0, 2, 3
};
int sizeOfIndices = sizeof(indices) / sizeof(int);