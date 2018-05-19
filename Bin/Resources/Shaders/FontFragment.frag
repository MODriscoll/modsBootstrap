#version 400 core

in vec2 fTexCoords;

uniform sampler2D textsymbol;
uniform vec4 textcolor;

out vec4 FragColor;

void main()
{
	vec4 texel = vec4(1.f, 1.f, 1.f, texture(textsymbol, fTexCoords).r);
	FragColor = textcolor * texel;
}