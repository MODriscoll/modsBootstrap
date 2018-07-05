#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in int vIndex;
layout (location = 2) in vec4 vColor;
layout (location = 3) in vec2 vTexCoords;

out int fIndex;
out vec4 fColor;
out vec2 fTexCoords;

#include "Resources/Shaders/Scene/Camera.glin"

void main()
{
	fIndex = vIndex;
	fColor = vColor;
	fTexCoords = vTexCoords;
	gl_Position = ortho * vec4(vPosition, 1.f);
}