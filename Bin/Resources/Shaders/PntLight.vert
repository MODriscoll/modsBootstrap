#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

struct DirectionalLight
{
	vec3 color;
	float ambientstrength;
	float diffusestrength;	
	
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	float ambientstrength;
	float diffusestrength;	
	
	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 color;
	float ambientstrength;
	float diffusestrength;
	
	vec3 position;
	vec3 direction;
	float innercutoff;
	float outercutoff;
	
	float constant;
	float linear;
	float quadratic;
};

layout (std140, binding = 1) uniform Lights
{
	DirectionalLight dirlights[4];
	PointLight pntlights[10];
	SpotLight sptlights[10];
	
	int dircount;
	int pntcount;
	int sptcount;
};

uniform int index;

void main()
{
	PointLight light = pntlights[index];
	
	fTexCoords = vTexCoords;

	float lightmax = max(max(light.color.r, light.color.b), light.color.g);
	float radius = (-light.linear - sqrt(light.linear * light.linear - 4 * light.quadratic * (light.constant - (256.f / 5.f) * lightmax))) / (2.f * light.quadratic);
	//float radius = 1.f;
	mat4 model = mat4(vec4(radius, 0.f, 0.f, 0.f),
	vec4(0.f, radius, 0.f, 0.f), vec4(0.f, 0.f, radius, 0.f),
	vec4(light.position, 1.f));
	gl_Position = projection * view * model * vec4(vPosition, 1.f);
}