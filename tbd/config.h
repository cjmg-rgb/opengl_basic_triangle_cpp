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

float vertices[] =
{
   -.5f, .5f, .0f, 1.f, 0.f, 0.f, 0.f, 1.f, // TOP-LEFT
   -.5f,-.5f, .0f, .0f, 1.f, 0.f, 0.f, 0.f, // BOT-LEFT
	.5f,-.5f, .0f, .0f, 0.f, 1.f, 1.f, 0.f, // BOT-RIGHT
	.5f, .5f, .0f, .3f, .7f, .9f, 1.f, 1.f  // TOP-RIGHT
};

unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3
};