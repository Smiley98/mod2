#version 440

out vec4 outColour;

uniform vec3 u_colour;

void main(){
	outColour = vec4(u_colour, 1.0f);
}