#version 440

in fData {
	vec4 colour;
	vec4 colourInverse;
} fIn;

out vec4 outColour;

uniform float u_clientHeight;

void main()
{	//Interpolated version, *must correct rasterizer interpolation regardless.*
	//outColour = mix(normalize(fIn.colour), normalize(fIn.colourInverse), gl_FragCoord.y / u_clientHeight);
	///*
	vec4 c1 = normalize(fIn.colour);
	vec4 c1i = vec4(1.0f) - c1;
	outColour = mix(c1, c1i, gl_FragCoord.y / u_clientHeight);//*/

}