 #version 440

layout(location = 0) in vec2 a_position;

out VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vOut;

const vec2 offset = vec2(0.5, 0.5);

void main()
{  
    //Map from (-1, 1) to (0, 1).
	vOut.texCoord.xy = a_position * offset + offset;

    //Outwards facing normals.
	vOut.normal = vec3(0.0, 0.0, 1.0);

    //Do camera stuff in the fragment shader?
    //vOut.posEye = vec3(0.0);

    //Add z and w coordinates.
	gl_Position = vec4(a_position, 0.0, 1.0);
}