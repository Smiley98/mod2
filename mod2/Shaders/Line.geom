#version 440 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

//uniform mat4 u_mvp;
//uniform vec3 u_p1;
//uniform vec3 u_p2;

void main() {
	//gl_Position = u_mvp * vec4(u_p1, 1.0);
	gl_Position = vec4(-0.5f, -0.75f, 0.0f, 1.0f);
	EmitVertex();
	
	//gl_Position = u_mvp * vec4(u_p2, 1.0);
	gl_Position = vec4(-0.5f, 0.75f, 0.0f, 1.0f);
	EmitVertex();
	
	EndPrimitive();
}