#version 440

in fData {
	vec4 colour;
} fIn;

out vec4 outColour;

void main()
{
	outColour = fIn.colour;
}