#version 450 core

layout (location = 0) out vec3 lColor;

#include "Resources/Shaders/Application.glin"
#include "Resources/Shaders/Lights/Lights.glin"
#include "Resources/Shaders/Scene/Geometry.glin"
#include "Resources/Shaders/Scene/Camera.glin"

uniform int index;

// Assuming shininess value is 32 (need to make target to pass that in

// Inverse square falloff as explained by Epic Games in:
// https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
float GetFalloff(float distance, float radius)
{
	float dbyr = distance / radius;
	float dbyr2 = dbyr * dbyr;
	float dbyr4 = dbyr2 * dbyr2;
	
	float numerator = pow(clamp(1.f - dbyr4, 0.f, 1.f), 2);
	float denominator = (distance * distance) + 1.f;
	
	return numerator / denominator;
}

void main()
{
	PointLight light = pntlights[index];
	
	vec2 texcoord = gl_FragCoord.xy / viewportsize;

	vec3 fPosition = texture(target.gPosition, texcoord).rgb;
	vec3 fNormal = texture(target.gNormal, texcoord).rgb;
	vec3 viewdir = normalize(position - fPosition);
	
	// Apply point light
	{
		vec3 displacement = light.position - fPosition;
		vec3 lightdir = normalize(displacement);
		
		// Diffuse
		float diff = max(dot(fNormal, lightdir), 0.f);
		vec3 diffuse = light.color * light.intensity * diff;
	
		// Specular
		vec3 reflectdir = reflect(-lightdir, fNormal);
		float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
		vec3 specular = light.color * light.intensity * spec * texture(target.gAlbedoSpec, texcoord).a;
		
		// Attenuation
		float attenuation = GetFalloff(length(displacement), light.radius);
		
		lColor = (diffuse + specular) * attenuation;
	}
}