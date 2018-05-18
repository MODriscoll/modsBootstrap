#version 400 core

layout (location = 0) in vec3 vPosition;

uniform vec3 position;		// World space position

// Size of billboard
// If using Billboard3D_Scaled, this value should be half extents
// If using Billboard3D_Fixed, this value can be percentage of screen space
// or size in pixels (size in pixels = (screen size in pixels / billboard size in pixels))
uniform vec2 size;	

uniform mat4 projection;
uniform mat4 view;

vec4 Billboard3D_Scaled()
{
	vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
	
	vec3 pos = position;
	pos += right * vPosition.x * size.x;
	pos += up * vPosition.y * size.y;
	
	return projection * view * vec4(pos, 1.f);
}

vec4 Billboard3D_Fixed()
{
	vec4 pos = projection * view * vec4(position, 1.f);
	
	// Have to perspective division ourselves
	pos /= pos.w;
	
	pos.xy += vPosition.xy * size;
	
	return pos;
}

// TODO:
//vec4 Billboard3D_VerticalRotation()

void main()
{
	gl_Position = Billboard3D_Fixed();
}