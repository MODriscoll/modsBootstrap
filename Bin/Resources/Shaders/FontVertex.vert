#version 400 core

// vec4(pos.x, pos.y, texcoords.x, texcoords.y)
layout (location = 0) in vec4 vData;

out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
	fTexCoords = vData.zw;
	gl_Position = projection * vec4(vData.xy, 0.f, 1.f);
}