#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in Ray {
	vec4 colour;
	float height;
	float x;
} rayIn[];

out fData {
	vec4 colour;
} rayOut;

in float pos[];

uniform float u_halfScreenHeight;
//Faster to multiply than divide :D
//uniform float u_halfScreenHeightInverse;

void main()
{	//Screen space to NDC = divide by half resolution, then subtract 1.
	/*float halfRayHeight = rayIn[0].height * 0.5f;
	
	gl_Position = vec4(rayIn[0].x, (u_halfScreenHeight + halfRayHeight) / u_halfScreenHeight - 1.0f, 0.0, 1.0);
	//gl_Position = vec4(rayIn[0].x, (u_halfScreenHeight + halfRayHeight) * u_halfScreenHeightInverse - 1.0f, 0.0, 1.0);
	rayOut.colour = rayIn[0].colour;
	EmitVertex();
	
	gl_Position = vec4(rayIn[0].x, (u_halfScreenHeight - halfRayHeight) / u_halfScreenHeight - 1.0f, 0.0, 1.0);
	//gl_Position = vec4(rayIn[0].x, (u_halfScreenHeight + halfRayHeight) * u_halfScreenHeightInverse - 1.0f, 0.0, 1.0);
	rayOut.colour = vec4(1.0f) - rayIn[0].colour;
	EmitVertex();
	
	EndPrimitive();*/
	float halfHeight = (rayIn[0].height / 50.0f - 1.0f) * 0.5f;
	gl_Position = vec4(pos[0], -halfHeight, 0.0f, 1.0f);
	rayOut.colour = rayIn[0].colour;
	//rayOut.colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	EmitVertex();
	
	gl_Position = vec4(pos[0], halfHeight, 0.0f, 1.0f);
	rayOut.colour = vec4(1.0f) - rayIn[0].colour;
	//rayOut.colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();
	
	EndPrimitive();
}