#version 450 core

out vec4 color;
in vec3 v_normal;

void main()
{
	vec3 N = normalize(v_normal);
	color = vec4(0.5 * N + 0.5, 1.0);
}