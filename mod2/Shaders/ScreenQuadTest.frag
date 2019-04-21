#version 440

in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

out vec4 outColour;

void main() {
    //Fragment coordinates are in screen space so they must be divided by the screen resultion in order to be used the same way as texture coordinates.
	//vec2 fragmentCoordinates = gl_FragCoord.xy;
	vec2 textureCoordinates = vIn.texCoord.xy;
    outColour = vec4(textureCoordinates.xy, 0.0, 1.0);
}