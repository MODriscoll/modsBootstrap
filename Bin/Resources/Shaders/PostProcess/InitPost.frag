#version 450 core

layout (location = 0) out vec3 pNoPost;
layout (location = 1) out vec3 pBright;

in vec2 fTexCoords;

// Textures containing the geometry
// and lighting from respective passes
struct Targets
{
	sampler2D gAlbedoSpec;
	sampler2D lColor;
};

uniform Targets target;

// Bloom
uniform bool bBloom;
uniform float brightnessThreshold;

void main()
{
	vec3 fAlbedo = texture(target.gAlbedoSpec, fTexCoords).rgb;
	vec3 lColor = texture(target.lColor, fTexCoords).rgb;

	// Scene with no post processing applied
	pNoPost = fAlbedo * lColor;
	
	// Human eye is more sensitive to green then other colors
	float brightness = dot(pNoPost, vec3(0.2126f, 0.7152f, 0.0722f));
	if (brightness > brightnessThreshold)
		pBright = pNoPost * float(bBloom);
}