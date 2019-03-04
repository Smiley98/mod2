#version 440

in fData {
	vec4 colour;
	vec4 colourInverse;
} fIn;

out vec4 outColour;

uniform float u_clientHeight;

void main()
{
	outColour = mix(fIn.colour, fIn.colourInverse, gl_FragCoord.y / u_clientHeight);
}