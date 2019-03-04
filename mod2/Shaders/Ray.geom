#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in Ray {
	vec4 colour;
	float halfHeightNdc;
	float x;
} rayIn[];

out fData {
	vec4 colour;
} rayOut;

void main()
{
	gl_Position = vec4(rayIn[0].x, rayIn[0].halfHeightNdc, 0.0f, 1.0f);
	rayOut.colour = rayIn[0].colour;
	EmitVertex();
	
	gl_Position = vec4(rayIn[0].x, -rayIn[0].halfHeightNdc, 0.0f, 1.0f);
	rayOut.colour = vec4(1.0f) - rayIn[0].colour;
	EmitVertex();
	
	EndPrimitive();
}