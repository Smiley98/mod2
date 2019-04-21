#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;		//Ray colour.
layout(location = 1) in float a_halfHeight;	//Half-height of ray in screen space.

out Ray {
	vec4 colour;
	float halfHeight;
	float x;
} ray;

uniform float u_start;	//Ndc.
uniform float u_step;	//Ndc.

void main()
{
	ray.colour = a_colour;
	//Can't do ndc conversion here cause more screen space stuff needs to happen first.
	ray.halfHeight = a_halfHeight;
	ray.x = u_start + u_step * float(gl_VertexID);
}