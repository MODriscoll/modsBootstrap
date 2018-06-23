#version 450 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform mat4 projection;

#include "Resources/Shaders/Application.glin"

void main()
{
	fTexCoords = vTexCoords;
	gl_Position = projection * vec4(vTexCoords * viewportsize, 0.f, 1.f);
}