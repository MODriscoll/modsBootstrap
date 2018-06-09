#version 450 core

out vec4 fragcolor;

in vec2 fTexCoords;

// Textures containing the geometry
// and lighting from respective passes
struct Targets
{
	sampler2D gPosition;
	sampler2D gNormal;
	sampler2D gAlbedoSpec;
	sampler2D lColor;
};

uniform Targets target;

// Gamma correction
// TODO: move to its own shader?
// as it should be done very last
uniform bool bGammaCorrect;
uniform float gamma;

// HDR tonemapping
// Needs to be calculated before gamma correction
// as it converts HDR to LDR
uniform float exposure;

void main()
{
	vec3 fAlbedo = texture(target.gAlbedoSpec, fTexCoords).rgb;
	vec3 lColor = texture(target.lColor, fTexCoords).rgb;

	vec3 color = lColor;
	
	// Tone mapping
	color = vec3(1.f) - exp(-color * exposure);
	
	color = fAlbedo * color;

	// Gamma correction
	if (bGammaCorrect)
	{
		color = pow(color, vec3(1.f / gamma));
	}
	
	fragcolor = vec4(color, 1.f);
}