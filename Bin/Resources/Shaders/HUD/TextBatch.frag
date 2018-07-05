#version 450 core

out vec4 fragcolor;

flat in int fIndex;
in vec4 fColor;
in vec2 fTexCoords;

#ifndef MAX_FONTS_PER_BATCH
#define MAX_FONTS_PER_BATCH 16
#endif

uniform sampler2D atlas[MAX_FONTS_PER_BATCH];

void main()
{
	vec4 color = fColor;
	color.a *= texture(atlas[fIndex], fTexCoords).r;
	fragcolor = color;
}