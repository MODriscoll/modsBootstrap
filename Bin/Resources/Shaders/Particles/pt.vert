#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 gColor;

uniform mat4 model;

void main()
{
	gColor = vColor;

	gl_Position = model * vec4(vPosition, 1.f);
}