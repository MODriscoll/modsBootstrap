#version 450 core

layout (location = 0) out vec3 lColor;

in vec2 fTexCoords;

// Textures containing the geometry
// data from the geometry pass
struct GTargets
{
	sampler2D gPosition;
	sampler2D gNormal;
	sampler2D gAlbedoSpec;
};

uniform GTargets target;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
	vec3 heading;
};

struct DirectionalLight
{
	vec3 color;	
	float intensity;
	vec3 direction;	
};

#ifndef MAX_DIRECTIONAL_LIGHTS
#define MAX_DIRECTIONAL_LIGHTS 4
#endif

struct PointLight
{
	vec3 color;
	float intensity;
	vec3 position;	
	float radius;
};

#ifndef MAX_POINT_LIGHTS
#define MAX_POINT_LIGHTS 10
#endif

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float intensity;
	float radius;
	float inner;
	float outer;
};

#ifndef MAX_SPOT_LIGHTS
#define MAX_SPOT_LIGHTS 10
#endif

layout (std140, binding = 1) uniform Lights
{
	DirectionalLight[MAX_DIRECTIONAL_LIGHTS] dirlights;
	PointLight[MAX_POINT_LIGHTS] pntlights;
	SpotLight[MAX_SPOT_LIGHTS] sptlights;
	
	int dircount;
	int pntcount;
	int sptcount;
};

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
	
	// Size of viewport, should be a uniform
	vec2 gScreenSize = vec2(1280.f, 720.f);

	vec3 fPosition = texture(target.gPosition, gl_FragCoord.xy / gScreenSize).rgb;
	vec3 fNormal = texture(target.gNormal, gl_FragCoord.xy / gScreenSize).rgb;
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
		vec3 specular = light.color * light.intensity * spec * texture(target.gAlbedoSpec, gl_FragCoord.xy / gScreenSize).a;
		
		// Attenuation
		float attenuation = GetFalloff(length(displacement), light.radius);
		
		lColor = (diffuse + specular) * attenuation;
	}
}