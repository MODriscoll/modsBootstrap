#version 450 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

#include "Resources/Shaders/Scene/Camera.glin"

void main()
{
	fTexCoords = vPosition;
	mat4 view_notrans = mat4(mat3(view));
	vec4 pos = projection * view_notrans * vec4(vPosition, 1.f);
	
	// Results in z value always being 1
	// z will get set to w which will result in (w / w = 1)
	// depth function will need to be set to GL_LEQUAL for this shader to work
	gl_Position = pos.xyww;
}