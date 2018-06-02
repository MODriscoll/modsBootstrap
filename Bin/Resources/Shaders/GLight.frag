#version 450 core

out vec4 fragcolor;

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
	vec4 color;
	float ambientstrength;
	float specularstrength;	
	
	vec3 direction;
};

struct PointLight
{
	vec4 color;
	float ambientstrength;
	float specularstrength;	
	
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec4 color;
	float ambientstrength;
	float specularstrength;
	
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

// Just using lights RGB values (need to check if they should have an alpha)
// Also assuming shininess value is 32 (need to make target to pass that in

float GetAttenuation(float distance, float constant, float linear, float quadratic)
{
	return 1.f / (constant + linear * distance + quadratic * (distance * distance));
}

vec3 ApplyDirectionalLight(DirectionalLight light, vec3 fNormal, vec3 viewdir)
{
	vec3 lightdir = normalize(-light.direction);
	
	// Diffuse
	float diff = max(dot(fNormal, lightdir), 0.f);
	vec3 diffuse = light.color.rgb * diff * texture(target.gAlbedoSpec, fTexCoords).rgb;
	
	// Specular
	vec3 reflectdir = reflect(-lightdir, fNormal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
	vec3 specular = light.color.rgb * spec * texture(target.gAlbedoSpec, fTexCoords).a;
	
	return diffuse + specular;
}

vec3 ApplyPointLight(PointLight light, vec3 fPosition, vec3 fNormal, vec3 viewdir)
{
	vec3 displacement = light.position - fPosition;
	vec3 lightdir = normalize(displacement);
	
	// Diffuse
	float diff = max(dot(fNormal, lightdir), 0.f);
	vec3 diffuse = light.color.rgb * diff * texture(target.gAlbedoSpec, fTexCoords).rgb;
	
	// Specular
	vec3 reflectdir = reflect(-lightdir, fNormal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
	vec3 specular = light.color.rgb * spec * texture(target.gAlbedoSpec, fTexCoords).a;
	
	// Attenuation
	float attenuation = GetAttenuation(length(displacement), light.constant, light.linear, light.quadratic);
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

vec3 ApplySpotLight(SpotLight light, vec3 fPosition, vec3 fNormal, vec3 viewdir)
{
	vec3 displacement = light.position - fPosition;
	vec3 lightdir = normalize(displacement);
	
	// Diffuse
	float diff = max(dot(fNormal, lightdir), 0.f);
	vec3 diffuse = light.color.rgb * diff * texture(target.gAlbedoSpec, fTexCoords).rgb;
	
	// Specular
	vec3 reflectdir = reflect(-lightdir, fNormal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.f), 32);
	vec3 specular = light.color.rgb * spec * texture(target.gAlbedoSpec, fTexCoords).a;
	
	// Soft edges
	float theta = dot(lightdir, normalize(-light.direction));
	float epsilon = (light.innercutoff - light.outercutoff);
	float intensity = clamp((theta - light.outercutoff) / epsilon, 0.f, 1.f);
	
	diffuse *= intensity;
	specular *= intensity;
	
	// Attenuation
	float attenuation = GetAttenuation(length(displacement), light.constant, light.linear, light.quadratic);
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

void main()
{
	vec3 fPosition = texture(target.gPosition, fTexCoords).rgb;
	vec3 fNormal = texture(target.gNormal, fTexCoords).rgb;
	vec3 viewdir = normalize(position - fPosition);
	
	// Apply lighting
	vec3 lighting = vec3(0.f);
	for (int i = 0; i < dircount; ++i)
	{
		lighting += ApplyDirectionalLight(dirlights[i], fNormal, viewdir);
	}
	
	for (int i = 0; i < pntcount; ++i)
	{
		lighting += ApplyPointLight(pntlights[i], fPosition, fNormal, viewdir);
	}
	
	for (int i = 0; i < sptcount; ++i)
	{
		lighting += ApplySpotLight(sptlights[i], fPosition, fNormal, viewdir);
	}
	
	fragcolor = vec4(lighting, 1.f);
}