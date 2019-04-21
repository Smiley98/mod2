#version 440

in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

out vec4 outColour;

void main() {
	vec2 coords = gl_FragCoord.xy;
	vec2 coords2 = vIn.texCoord.xy;
    outColour = vec4(coords2, 0.0, 1.0);
}