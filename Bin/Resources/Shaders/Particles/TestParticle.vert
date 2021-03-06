#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoords;

out vec3 fPosition;
out vec3 fColor;
out vec3 fNormal;
out vec2 fTexCoords;

#include "Resources/Shaders/Scene/Camera.glin"

void main()
{
	fPosition = vPosition;
	fColor = vColor;
	fNormal = vNormal;
	fTexCoords = vTexCoords;
	
	gl_Position = projection * view * vec4(vPosition, 1.f);
}
