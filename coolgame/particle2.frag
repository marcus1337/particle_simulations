#version 450 core

out vec4 f_color;

in vec3 color;
in float alpha;

flat in vec2 centre;
flat in float radiusPixels;

const vec3 lightDir = vec3(0,0,1.0);

void main()
{
    vec3 N;
    N.xy = gl_PointCoord * 2.0 - vec2(1.0);    
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    ///N.z = sqrt(1.0-mag);
    ///float diffuse = max(0.0, dot(lightDir, N));
    ///color = vec4(v_color * diffuse,1.0);
    //color.a = v_color.a;

	vec2 coord = (gl_FragCoord.xy - centre) / radiusPixels;
	float l = length(coord);
	if (l > 1.0)
        discard;
	vec3 pos = vec3(coord, sqrt(1.0-l*l));
    f_color = vec4(vec3(pos.z)*color, alpha);

    //vec2 p = gl_PointCoord * 2.0 - vec2(1.0);
    //if (dot(p, p) > 1.0) {
    //   discard;
    //}
	//vec3 pos = vec3(0,0, sqrt(1.0-p*p));
	//color = vec4(vec3(pos.z)*color, 0.5);


}