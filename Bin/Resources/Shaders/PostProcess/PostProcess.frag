#version 450 core

out vec4 fragcolor;

in vec2 fTexCoords;

// Targets from previous post processing passes
struct Targets
{
	sampler2D pNoPost;
	sampler2D pBloom;
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
	vec3 pNoPost = texture(target.pNoPost, fTexCoords).rgb;
	vec3 pBloom = texture(target.pBloom, fTexCoords).rgb;
	
	// Additive blending
	vec3 color = pNoPost + pBloom;
	
	// Tone mapping
	color = vec3(1.f) - exp(-color * exposure);

	// Gamma correction
	if (bGammaCorrect)
	{
		color = pow(color, vec3(1.f / gamma));
	}
	
	fragcolor = vec4(color, 1.f);
}