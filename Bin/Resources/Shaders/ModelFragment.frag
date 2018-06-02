#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
};

uniform Material material;

void main()
{
	gPosition = fPosition;
	gNormal = fNormal;
	gAlbedoSpec.rgb = texture(material.diffuse1, fTexCoords).rgb;
	gAlbedoSpec.a = texture(material.specular1, fTexCoords).r;
}