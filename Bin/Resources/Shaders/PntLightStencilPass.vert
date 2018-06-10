#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

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
	float intensity;
	vec3 direction;	
};

#ifndef MAX_DIRECTIONAL_LIGHTS
#define MAX_DIRECTIONAL_LIGHTS 4
#endif

struct PointLight
{
	vec3 color;
	float intensity;
	vec3 position;	
	float radius;
};

#ifndef MAX_POINT_LIGHTS
#define MAX_POINT_LIGHTS 10
#endif

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float intensity;
	float radius;
	float inner;
	float outer;
};

#ifndef MAX_SPOT_LIGHTS
#define MAX_SPOT_LIGHTS 10
#endif

layout (std140, binding = 1) uniform Lights
{
	DirectionalLight[MAX_DIRECTIONAL_LIGHTS] dirlights;
	PointLight[MAX_POINT_LIGHTS] pntlights;
	SpotLight[MAX_SPOT_LIGHTS] sptlights;
	
	int dircount;
	int pntcount;
	int sptcount;
};

uniform int index;
//uniform mat4 model;

void main()
{
	PointLight light = pntlights[index];
	
	// Construct model matrix using
	// the lights position and radius
	mat4 model = mat4(
		vec4(light.radius, 0.f, 0.f, 0.f),
		vec4(0.f, light.radius, 0.f, 0.f), 
		vec4(0.f, 0.f, light.radius, 0.f),
		vec4(light.position, 1.f));
		
	gl_Position = projection * view * model * vec4(vPosition, 1.f);
}