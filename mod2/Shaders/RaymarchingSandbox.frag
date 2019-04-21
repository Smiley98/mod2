#version 440

//Interface block.
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

//Output colour.
out vec4 outColour;

//Raymarching constants:
const int MAX_MARCHING_STEPS = 255;
//Near plane.
const float MIN_DIST = 0.0;
//Far plane.
const float MAX_DIST = 100.0;
//Small number to account for floating point error.
const float EPSILON = 0.0001;

//Signed distance function for a sphere centered at the origin with an arbitrary radius.
float sphereSDF(vec3 position, float radius) {
    return length(position) - radius;
}

//Function describing all the scene geometry (currently just one circle with a radius of one about the origin).
float sceneSDF(vec3 samplePoint) {
    return sphereSDF(samplePoint, 1.0);
}

//Distance between the ray and surface geometry.
float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(eye + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

uniform vec2 u_resolution;

void main() {
	//vec2 textureCoordinates = vIn.texCoord.xy;
    //outColour = vec4(textureCoordinates.xy, 1.0, 1.0);
	vec3 dir = rayDirection(45.0, u_resolution, gl_FragCoord.xy);
    vec3 eye = vec3(0.0, 0.0, 5.0);
    float dist = shortestDistanceToSurface(eye, dir, MIN_DIST, MAX_DIST);

	if (dist > MAX_DIST - EPSILON) {
        outColour = vec4(0.0, 0.0, 0.0, 1.0);
		return;
    }
    
    outColour = vec4(1.0, 0.0, 0.0, 1.0);
}

 //Args: 1. Vertical FoV 2. Screen resolution. 3. Current fragment coordinate (screen space).
 //Make this better by unbiasing the texture instead.
 //This is essentially the raycasting camera; A large projection plan or narrow FoV pushes the viewer backwards.
 //TODO: Try doing this out of the box, then try doing it my own way to see if things match up.
 //Literally all I need to do is find a way to come up with a normalized direction from camera to fragment (ray direction).