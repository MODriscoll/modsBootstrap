#version 450 core

in vec3 fTexCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
	FragColor = texture(skybox, fTexCoords);
}