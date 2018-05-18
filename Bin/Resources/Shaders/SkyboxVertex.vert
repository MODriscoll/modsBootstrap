#version 400 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	fTexCoords = vPosition;
	vec4 pos = projection * view * vec4(vPosition, 1.f);
	
	// Results in z value always being 1
	// z will get set to w which will result in (w / w = 1)
	// depth function will need to be set to GL_LEQUAL for this shader to work
	gl_Position = pos.xyww;
}