#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 gColor[];

out vec3 fPosition;
out vec3 fColor;
out vec3 fNormal;
out vec2 fTexCoords;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

uniform float size;

vec4 ApplyOffset(vec3 right, vec3 up, float extx, float exty)
{
	vec3 position = gl_in[0].gl_Position.xyz;
	
	position += right * extx;
	position += up * exty;

	return vec4(position, 1.f);
}

void main()
{
	fColor = gColor[0];
	fNormal = -heading;

	// For scaled billboards, the half size needs to be used
	float hs = size * 0.5f;
	
	mat4 projview = projection * view;
	
	vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
	
	gl_Position = projview * ApplyOffset(right, up, -hs, hs);
	fPosition = gl_Position.xyz;
	fTexCoords = vec2(0.f, 1.f);
	EmitVertex();
	
	gl_Position = projview * ApplyOffset(right, up, -hs, -hs);
	fPosition = gl_Position.xyz;
	fTexCoords = vec2(0.f, 0.f);
	EmitVertex();
	
	gl_Position = projview * ApplyOffset(right, up, hs, hs);
	fPosition = gl_Position.xyz;
	fTexCoords = vec2(1.f, 1.f);
	EmitVertex();
	
	gl_Position = projview * ApplyOffset(right, up, hs, -hs);
	fPosition = gl_Position.xyz;
	fTexCoords = vec2(1.f, 0.f);
	EmitVertex();
	
	EndPrimitive();	
}