#version 440

in vec3 fs_position;
in vec3 fs_color;
in vec2 fs_texcoord;
in vec3 fs_normal;

out vec4 FragmentColor;

uniform sampler2D texture0;
uniform vec3 lightPos0;

void main()
{
 
	//FragmentColor = vec4(fs_color, 1.f);

	// Ambient Light
	vec3 ambientLight = vec3(.1f, .1f, .1f);

	// Diffuse Light
	vec3 posToLightDirVec = normalize(lightPos0 - fs_position);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, fs_normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;


	FragmentColor = texture(texture0, fs_texcoord) * vec4(fs_color, 1.f)
	* (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f));
}
