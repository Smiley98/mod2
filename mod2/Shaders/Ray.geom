#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in Ray {
	vec4 colour;
	float halfHeight;
	float x;
} rayIn[];

out fData {
	vec4 colour;
	vec4 colourInverse;
} rayOut;

uniform float u_rayOrigin;
uniform float u_clientY;
uniform float u_clientHeight;

float screenToNdcY(float y) {
	return (y - u_clientY) / (u_clientHeight * 0.5f) - 1.0f; 
}

void main()
{
	gl_Position = vec4(rayIn[0].x, screenToNdcY(u_rayOrigin + rayIn[0].halfHeight), 0.0f, 1.0f);
	rayOut.colour = rayIn[0].colour;
	EmitVertex();
	
	gl_Position = vec4(rayIn[0].x, screenToNdcY(u_rayOrigin - rayIn[0].halfHeight), 0.0f, 1.0f);
	rayOut.colourInverse = vec4(1.0f) - rayIn[0].colour;
	EmitVertex();
	
	EndPrimitive();
}