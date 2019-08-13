#version 450 core

in vec2 uv_frag;
in float alpha;

uniform sampler2D tex;

out vec4 frag_color;

void main(){
    frag_color = texture(tex, uv_frag);
	//frag_color.w = alpha;

}