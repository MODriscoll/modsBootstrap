#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 gColor[];

out vec3 fPosition;
out vec3 fColor;
out vec3 fNormal;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

uniform float size;

void main()
{
	fColor = gColor[0];

	float halfsize = size * 0.5f;
	
	vec3 corners[4];
	corners[0] = gl_in[0].gl_Position.xyz + vec3(-halfsize, halfsize, 0);
	corners[1] = gl_in[0].gl_Position.xyz + vec3(halfsize, halfsize, 0);
	corners[2] = gl_in[0].gl_Position.xyz + vec3(halfsize, -halfsize, 0);
	corners[3] = gl_in[0].gl_Position.xyz + vec3(-halfsize, -halfsize, 0);
	
	mat3 billboard;
	billboard[2] = normalize(position - gl_in[0].gl_Position.xyz);
	billboard[0] = cross(vec3(0.f, 1.f, 0.f), billboard[2]);
	billboard[1] = cross(billboard[2], billboard[0]);
	
	fNormal = billboard[2];
	
	mat4 projview = projection * view;
	
	for (int i = 0; i < 4; ++i)
	{
		fPosition = (projview * vec4(billboard * corners[i], 1.f)).xyz;
	
		gl_Position = projview * vec4(billboard * corners[i], 1.f);
		EmitVertex();
	}
}