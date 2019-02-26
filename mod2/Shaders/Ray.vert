#version 440
//'a' for "attribute", like in the gl 2.0 days!
layout(location = 0) in vec4 a_colour;
layout(location = 1) in float a_height;

out Ray {
	vec4 colour;
	float height;
	float x;
} ray;

//Working in NDC would be nice for x cause step is from -1.0f to 1.0f, but screen space is necessary for y plus 0 to 1920 is still very nice for x.
//(Plus instance_id or whatever is most likely an int that scales with primitives so it may even be the best!)
float x_position = 0.0f;
void main()
{	//Do += u_step instead if I want more control.
	ray.x = x_position;
	x_position += 1.0f;
	//Map from screen space to NDC:
	//I should've wrote these comments inside the geometry shader. All I can do here is pass on attributes.
	ray.colour = a_colour;
	ray.height = a_height;
}