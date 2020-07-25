#version 440

in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

out vec4 outColour;

uniform sampler2D u_texture;

void main() {
    outColour = texture(u_texture, vIn.texCoord.xy);
}