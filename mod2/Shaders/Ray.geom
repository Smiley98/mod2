#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in Ray {
	vec4 colour;
	float height;
	float x;
} rayIn[];

out fData {
	//Could have a 2nd colour for wacky visuals via rasterizer interpolation.
	vec4 colour;
} rayOut;

//I don't even need this assuming I want to start at 0 and cast 1 ray per horizontal pixel! (Could add start x and y if I really wanted extensibility).
//uniform int u_screenWidth;
uniform int u_screenHeight;

void main()
{	//Screen space to NDC = divide by half resolution, then subtract 1.
	//Ray ray = rayIn[0];
	float halfRayHeight = rayIn[0].height * 0.5f;
	float halfScreenHeight = float(u_screenHeight) * 0.5f;
	gl_Position = vec4(/*float(gl_InstanceID)*/rayIn[0].x, (halfScreenHeight + halfRayHeight) / halfScreenHeight - 1.0f, 0.0, 1.0);
	rayOut.colour = rayIn[0].colour;
	EmitVertex();

	gl_Position = vec4(/*float(gl_InstanceID)*/rayIn[0].x, (halfScreenHeight - halfRayHeight) / halfScreenHeight - 1.0f, 0.0, 1.0);
	rayOut.colour = rayIn[0].colour;
	EmitVertex();

	EndPrimitive();
}