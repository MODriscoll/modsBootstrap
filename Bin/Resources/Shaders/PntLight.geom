#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vData
{
	vec2 texcoords;
	float radius;
} g[];

out vec2 fTexCoords;

uniform float time;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main()
{
	vec4 expand = vec4(0.f);//vec4(GetNormal() * ((sin(time) + 1.f) / 2.f) * 2.f, 0.f);
	
	gl_Position = gl_in[0].gl_Position + expand;
	fTexCoords = g[0].texcoords;
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position + expand;
	fTexCoords = g[1].texcoords;
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position + expand;
	fTexCoords = g[2].texcoords;
	EmitVertex();
	
	EndPrimitive();
}