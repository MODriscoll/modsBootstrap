#version 450 core

in vec2 fTexCoords;

uniform sampler2D atlas;

out vec4 FragColor;

void main()
{
	vec4 texel = vec4(1.f, 1.f, 1.f, texture(atlas, fTexCoords).r);
	FragColor = texel;
}