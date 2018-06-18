#version 400 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fPosition;
in vec3 fColor;
in vec3 fNormal;

void main()
{
	gPosition = fPosition;
	gNormal = fNormal;
	gAlbedoSpec = vec4(fColor, 1.f);
}