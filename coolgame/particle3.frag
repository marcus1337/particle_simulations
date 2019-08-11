#version 450 core

out vec4 f_color;
in vec4 color;

flat in vec2 centre;
flat in float radiusPixels;

const vec3 lightDir = vec3(0,0,1.0);

void main()
{
f_color = vec4(vec3(color.x,color.y,color.z), 1);

  //  vec3 N;
  //  N.xy = gl_PointCoord * 2.0 - vec2(1.0);    
  //  float mag = dot(N.xy, N.xy);
//    if (mag > 1.0) 
//		discard;

	//vec2 coord = (gl_FragCoord.xy - centre) / radiusPixels;
	//float l = length(coord);
	//if (l > 1.0)
     //   discard;
	//vec3 pos = vec3(coord, sqrt(1.0-l*l));
   // f_color = vec4(vec3(pos.z)*vec3(color.x,color.y,color.z), color.w);

}