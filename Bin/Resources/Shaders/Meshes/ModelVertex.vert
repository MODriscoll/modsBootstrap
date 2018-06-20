#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

out vData
{
	vec3 position;
	mat3 tbn;
	vec2 texcoords;
} v;

#include "Resources/Shaders/Scene/Camera.glin"

uniform mat4 model;
uniform mat3 normat;

void main()
{
	// Position of vertex in world
	vec4 world = model * vec4(vPosition, 1.f);

	v.position = world.xyz;
	
	// Calculate normal (using gram-schmidt)
	{
		vec3 T = normalize(normat * vTangent);
		vec3 N = normalize(normat * vNormal);
		
		// Re-orthogonalize tangent with respect to the normal
		T = normalize(T - dot(T, N) * N);
		
		// Retrieve perpendicular bitangent 
		vec3 B = cross(N, T);
		
		v.tbn = mat3(T, B, N);
	}
	
	v.texcoords = vTexCoords;
	
	gl_Position = projection * view * world;
}