#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 fPosition;
out vec3 fColor;
out vec3 fNormal;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

uniform mat4 model;

void main()
{
	vec4 world = model * vec4(vPosition, 1.f);
	
	fPosition = world.xyz;
	fColor = vColor;
	fNormal = model[2].xyz;
	
	gl_Position = projection * view * world;
}