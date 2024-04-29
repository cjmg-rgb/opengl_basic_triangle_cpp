#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 fs_color;
out vec2 fs_texcoord;

void main() 
{
	fs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	fs_color = vertex_color;

	gl_Position = vec4(vertex_position, 1);	
}