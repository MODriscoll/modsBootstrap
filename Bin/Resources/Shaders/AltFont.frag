#version 450 core

out vec4 fragcolor;

in vec4 fColor;
in vec2 fTexCoords;

uniform sampler2D atlas;

void main()
{
	vec4 color = fColor;
	color.a *= texture(atlas, fTexCoords).r;
	
	fragcolor = color;
}