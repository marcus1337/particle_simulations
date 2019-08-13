#version 450 core
#extension GL_ARB_explicit_attrib_location : require


layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

out vec2 uv_frag;
uniform mat4 MVP;
out float alpha;

void main(){
    uv_frag = uv;
    gl_Position = vec4(vertex, 1.0);
    gl_Position = MVP * gl_Position;
}