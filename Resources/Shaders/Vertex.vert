#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 aTexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	aTexCoords = aTex;
	gl_Position = projection * view * model * vec4(aPos, 1.f);
}