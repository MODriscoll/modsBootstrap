#version 450 core

layout (location = 0) out vec3 lColor;

#include "Resources/Shaders/Lights/Lights.glin"
#include "Resources/Shaders/Scene/Geometry.glin"
#include "Resources/Shaders/Scene/Camera.glin"

in vec2 fTexCoords;

uniform int index;

// Assuming shininess value is 32 (need to make target to pass that in

void main()
{
	DirectionalLight light = dirlights[index];

	vec3 fPosition = texture(target.gPosition, fTexCoords).rgb;
	vec3 fNormal = texture(target.gNormal, fTexCoords).rgb;
	vec3 viewdir = normalize(position - fPosition);
	
	// Apply directional light
	{
		vec3 lightdir = normalize(-light.direction);
		
		// Diffuse
		float diff = max(dot(fNormal, lightdir), 0.f);
		vec3 diffuse = light.color * light.intensity * diff;
		
		// Specular
		vec3 reflectdir = reflect(-lightdir, fNormal);
		float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
		vec3 specular = light.color * light.intensity * spec * texture(target.gAlbedoSpec, fTexCoords).a;
		
		lColor = diffuse + specular;
	}
}