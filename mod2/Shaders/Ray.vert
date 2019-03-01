#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;
layout(location = 1) in float a_height;

out Ray {
	vec4 colour;
	float height;
	float x;
} ray;

out float pos;


const float c_start = -1.0f;
const float c_step = 2.0f / 800.0f;//Replace with u_xRes.

void main()
{
	ray.colour = a_colour;
	ray.height = a_height;
	ray.x = c_start + c_step * float(gl_VertexID);
	pos = ray.x;
}