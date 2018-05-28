#version 400 core

in vec3 fNormal;
in vec2 fTexCoords;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
};

uniform Material material;

out vec4 FragColor;

void main()
{
	vec4 dif = texture(material.diffuse1, fTexCoords);
	vec4 spc = vec4(0.f);//texture(material.specular1, fTexCoords);
	FragColor = dif + spc;
}