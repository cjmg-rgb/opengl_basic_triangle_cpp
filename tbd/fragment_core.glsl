
#version 440

in vec3 fs_pos;
in vec3 fs_col;
in vec2 fs_tex;

out vec4 FragmentShader;

uniform sampler2D texture0;

void main()
{
	
	//FragmentShader = vec4(fs_col, 1.f);
	FragmentShader = texture(texture0, fs_tex);
}