#version 440

out float pos;

const float _start = -1.0f;
const float _step = 2.0f / 800.0f;

void main() {
	pos = _start + _step * float(gl_VertexID);
}