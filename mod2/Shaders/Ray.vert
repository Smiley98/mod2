#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;
layout(location = 1) in float a_height;

out Ray {
	vec4 colour;
	float height;
	float x;
} ray;

//Working in screen space then converting to NDC is the way to do 2d line rendering! No reason to work in world space.
float x_position = 0.0f;//Let's hope this resets each draw call.
void main()
{	//Do += u_step instead if I want more control.
	ray.x = x_position;
	x_position += 1.0f;
	//Map from screen space to NDC:
	//I should've wrote these comments inside the geometry shader. All I can do here is pass on attributes.
	ray.colour = a_colour;
	ray.height = a_height;
}