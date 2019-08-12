#version 450 core
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 v_color;
layout (location = 2) in float radius;

uniform mat4 MVP;
uniform float P11;
uniform vec2 u_screenSize;

out vec4 color;
flat out vec2 centre;
flat out float radiusPixels;

void main()
{

	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	gl_Position = MVP * gl_Position;
	gl_PointSize = (u_screenSize.y *  P11 * radius) / gl_Position.w;

	radiusPixels = gl_PointSize / 2.0;
	centre = (0.5 * gl_Position.xy/gl_Position.w + 0.5) * u_screenSize;
	if(gl_PointSize <= 0)
		gl_PointSize = 1;

	//gl_PointSize= u_screenSize.x;
	color = v_color;

}