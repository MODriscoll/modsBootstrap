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

void main()
{
	vec3 fAlbedo = texture(target.gAlbedoSpec, fTexCoords).rgb;
	vec3 lColor = texture(target.lColor, fTexCoords).rgb;

	vec3 color = fAlbedo * lColor;
	
	// Gamma correction
	if (bGammaCorrect)
	{
		color = pow(color, vec3(1.f / gamma));
	}
	
	fragcolor = vec4(color, 1.f);
}