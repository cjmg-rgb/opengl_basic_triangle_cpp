#version 440

in vec3 fs_color;
in vec2 fs_texcoord;

out vec4 fragment_color;

uniform sampler2D texture1;

void main() 
{
	fragment_color = vec4(fs_color, 1);
	//fragment_color = texture(texture1, fs_texcoord) * vec4(fs_color, 1);
}
