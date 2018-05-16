#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 aTexCoords;

void main()
{
	aTexCoords = aTex;
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}