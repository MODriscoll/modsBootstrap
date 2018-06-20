#version 450 core

layout (location = 0) in vec3 vPosition;

#include "Resources/Shaders/Lights/Lights.glin"
#include "Resources/Shaders/Scene/Camera.glin"

uniform int index;

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