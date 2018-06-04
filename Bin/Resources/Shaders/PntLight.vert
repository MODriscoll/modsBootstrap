#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vData
{
	vec2 texcoords;
	float radius;
} v;

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
	
	float radius;
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
	
	float radius;
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

	v.texcoords = vTexCoords;
	v.radius = 1.f;//light.radius;
	mat4 model = mat4(vec4(1.f, 0.f, 0.f, light.position.x),
	vec4(0.f, 1.f, 0.f, light.position.y), vec4(0.f, 0.f, 1.f, light.position.z),
	vec4(0.f, 0.f, 0.f, 1.f));
	gl_Position = projection * view * model * vec4(vPosition, 1.f);
}