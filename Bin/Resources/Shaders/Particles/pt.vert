#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in float vExtent;

out vec3 gColor;
out float gExtent;

uniform mat4 model;

void main()
{
	gColor = vColor;
	gExtent = vExtent;

	gl_Position = model * vec4(vPosition, 1.f);
}