#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;	//Per-vertex colour!
layout(location = 1) in float a_height;	//Height in screen space.

out Ray {
	vec4 colour;
	float halfHeightNdc;
	float x;
} ray;

uniform float u_start;	//Ndc.
uniform float u_step;	//Ndc.
uniform float u_clientY;//Screen space.
uniform float u_halfClientHeightInverse;//Screen space.

void main()
{
	ray.colour = a_colour;
	ray.halfHeightNdc = ((a_height - u_clientY) * u_halfClientHeightInverse - 1.0f) * 0.5f;
	ray.x = u_start + u_step * float(gl_VertexID);
}