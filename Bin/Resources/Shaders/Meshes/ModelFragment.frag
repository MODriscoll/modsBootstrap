#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vData
{
	vec3 position;
	mat3 tbn;
	vec2 texcoords;
} f;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
	sampler2D normal1;
};

uniform Material material;

void main()
{
	gPosition = f.position;
	gNormal = f.tbn[2];//f.tbn * normalize(texture(material.normal1, f.texcoords).rgb * 2.f - 1.f);
	gAlbedoSpec.rgb = texture(material.diffuse1, f.texcoords).rgb;
	gAlbedoSpec.a = texture(material.specular1, f.texcoords).r;
}