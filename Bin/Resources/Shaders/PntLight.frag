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
	float ambientstrength;
	float diffusestrength;	
	
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	float ambientstrength;
	float diffusestrength;	
	
	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 color;
	float ambientstrength;
	float diffusestrength;
	
	vec3 position;
	vec3 direction;
	float innercutoff;
	float outercutoff;
	
	float constant;
	float linear;
	float quadratic;
};

layout (std140, binding = 1) uniform Lights
{
	DirectionalLight[4] dirlights;
	PointLight[10] pntlights;
	SpotLight[10] sptlights;
	
	int dircount;
	int pntcount;
	int sptcount;
};

uniform int index;

// Just using lights RGB values (need to check if they should have an alpha)
// Also assuming shininess value is 32 (need to make target to pass that in

float GetAttenuation(float distance, float constant, float linear, float quadratic)
{
	return 1.f / (constant + linear * distance + quadratic * (distance * distance));
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
		vec3 diffuse = light.color * diff;
	
		// Specular
		vec3 reflectdir = reflect(-lightdir, fNormal);
		float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
		vec3 specular = light.color * spec * texture(target.gAlbedoSpec, gl_FragCoord.xy / gScreenSize).a;
		
		// Attenuation
		float attenuation = GetAttenuation(length(displacement), light.constant, light.linear, light.quadratic);
		
		diffuse *= attenuation;
		specular *= attenuation;
		
		lColor = (diffuse + specular) * 20.f;
	}
}