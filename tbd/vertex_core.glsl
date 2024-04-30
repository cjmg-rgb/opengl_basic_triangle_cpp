#version 440

layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 vertex_col;
layout (location = 2) in vec2 vertex_tex;


out vec3 fs_pos;
out vec3 fs_col;
out vec2 fs_tex;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	
	fs_pos = vec4(ModelMatrix * vec4(vertex_pos, 1.f)).xyz;
	fs_col = vertex_col;
	fs_tex = vec2(vertex_tex.x, vertex_tex.y * -1.f);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_pos, 1.f);
}

