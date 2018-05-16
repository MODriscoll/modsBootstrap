#version 400 core

in vec2 aTexCoords;

uniform sampler2D u_texture;

out vec4 FragColor;
void main()
{
	vec4 texel = texture(u_texture, aTexCoords);
	FragColor = texel;
}