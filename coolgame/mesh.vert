#version 450 core
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec4 position;
layout(location = 1) in vec3 a_normal;

uniform mat4 MVP;
out vec3 v_normal;

void main()
{
	v_normal = a_normal;

	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	gl_Position = MVP * gl_Position;

}