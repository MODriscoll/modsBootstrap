#version 450 core

out vec4 fragcolor;

in vec2 fTexCoords;

// Textures containing the geometry
// and lighting from respective passes
struct Targets
{
	sampler2D gPosition;
	sampler2D gNormal;
	sampler2D gAlbedoSpec;
	sampler2D lColor;
};

uniform Targets target;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

void main()
{
	vec3 fAlbedo = texture(target.gAlbedoSpec, fTexCoords).rgb;
	vec3 lColor = texture(target.lColor, fTexCoords).rgb;

	fragcolor = vec4(lColor, 1.f);
}