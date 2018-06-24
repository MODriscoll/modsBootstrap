#version 450 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoords;

out vec4 fColor;
out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
	fColor = vColor;
	fTexCoords = vTexCoords;
	gl_Position = projection * vec4(vPosition, 0.f, 1.f);
}