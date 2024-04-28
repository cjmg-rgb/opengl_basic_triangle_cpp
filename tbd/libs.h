#pragma once

#include "iostream"
#include "vector"
#include "string"

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


struct Vertex
{
	glm::vec3 pos;
};

float triangle1[] =
{
	-.5f, .5f, .0f,
	-.8f, -.5f, .0f,
	-.2f, -.5f, .0f
};

float triangle2[] =
{
	.5f, .5f, .0f,
	.8f, -.5f, .0f,
	.2f, -.5f, .0f
};

const char* for_vertex_shader = R"glsl(
	#version 440

	layout (location = 0) in vec3 vertex_position;

	void main()
	{

		gl_Position = vec4(vertex_position, 1.0f);
	}

)glsl";	

const char* for_fragment_shader1 = R"glsl(
	#version 440

	out vec4 fs_color;

	void main()
	{
		fs_color = vec4(.5, .5, .5, 1.0);
	
	}

)glsl";

const char* for_fragment_shader2 = R"glsl(
	#version 440

	out vec4 fs_color;

	void main()
	{
		fs_color = vec4(1.0, 1.0, 0, 1.0);
	
	}

)glsl";
