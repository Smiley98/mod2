#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;	//Per-vertex colour!
layout(location = 1) in float a_height;	//Height in screen space.

out Ray {
	vec4 colour;
	float height;
	float x;
} ray;

uniform float u_start;	//Ndc.
uniform float u_step;	//Ndc.

void main()
{
	ray.colour = a_colour;
	//Can't do ndc conversion here cause more screen space stuff needs to happen first.
	ray.height = a_height;
	ray.x = u_start + u_step * float(gl_VertexID);
}