#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in float pos[];

void main() {
	gl_Position = vec4(pos[0], -0.75f, 0.0f, 1.0f);
	EmitVertex();
	
	gl_Position = vec4(pos[0], 0.75f, 0.0f, 1.0f);
	EmitVertex();
	
	EndPrimitive();
}