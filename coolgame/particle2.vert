#version 450 core
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 MVP;
uniform mat4 MV;
uniform float radius;

uniform vec3 v_color;
uniform float v_alpha;

uniform vec3 position;
uniform vec3 global_position;
uniform vec3 camera_position;

uniform mat4 P;

const vec2 vpSize = vec2(-1,1);
//const float rad = 0.5;
//const float dia = 100.0;

out vec3 color;
out float alpha;

flat out vec2 centre;
flat out float radiusPixels;

uniform vec2 u_screenSize;

void main()
{

	alpha = v_alpha;
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	gl_Position = MVP * gl_Position;

	//pixelSize = vpHeight * P[1][1] * radius / w_clip
	gl_PointSize = (u_screenSize.y *  P[1][1] * radius) / gl_Position.w;

	radiusPixels = gl_PointSize / 2.0;
	centre = (0.5 * gl_Position.xy/gl_Position.w + 0.5) * u_screenSize;
	if(gl_PointSize <= 0)
		gl_PointSize = 1;
	color = v_color;

}