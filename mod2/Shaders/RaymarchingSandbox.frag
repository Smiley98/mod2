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

uniform vec2 u_resolution;
uniform float u_time;

//Signed distance function for a sphere centered at the origin with an arbitrary radius.
float sphereSDF(vec3 point, float radius) {
    return length(point) - radius;
}

//Signed distance function for a cube centered at the origin with sidelengths of 2.
float cubeSDF(vec3 point, float sideLength) {
    //If all components of d are negative, then the point is inside the unit cube.
    vec3 d = abs(point) - vec3(sideLength);
    
    // Assuming p is inside the cube, how far is it from the surface?
    // Result will be negative or zero.
    float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);
    
    // Assuming p is outside the cube, how far is it from the surface?
    // Result will be positive or zero.
    float outsideDistance = length(max(d, 0.0));
    
    return insideDistance + outsideDistance;
}

//Function describing all the scene geometry (currently just one circle with a radius of one about the origin).
float sceneSDF(vec3 point) {
    float yAsX = u_resolution.x / u_resolution.y;
    float uniformTranslation = 0.25f;
    return cubeSDF(point - vec3(uniformTranslation, uniformTranslation * yAsX, 0.0), 0.75);// + sphereSDF(point, 1.0);
}

//Distance between the ray and surface geometry.
float marchScene(vec3 eye, vec3 marchingDirection, float start, float end) {
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

//Normalized ray direction based on the pinhole camera.
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
	//Formula = screen half width * tan half fov.
    vec2 xy = fragCoord - size * 0.5;
	//We're using y rather than x because we want to ensure we bound our scene based on the smaller of the two (the vertical; y).
    float z = size.y / tan(radians(fieldOfView) * 0.5);//Could pre-compute this because the field of view and size (screen resolution) don't change per-fragment.
    return normalize(vec3(xy, -z));
}

//Samples surrounding fragments to approximate the gradient which produces the steepest direction (normal).
vec3 estimateNormal(vec3 point) {
    return normalize(vec3(
        sceneSDF(vec3(point.x + EPSILON, point.y, point.z)) - sceneSDF(vec3(point.x - EPSILON, point.y, point.z)),
        sceneSDF(vec3(point.x, point.y + EPSILON, point.z)) - sceneSDF(vec3(point.x, point.y - EPSILON, point.z)),
        sceneSDF(vec3(point.x, point.y, point.z + EPSILON)) - sceneSDF(vec3(point.x, point.y, point.z - EPSILON))
    ));
}

vec3 phongPointLight(vec3 diffuse, vec3 specular, float alpha, vec3 point, vec3 eye, vec3 lightPos) {
    //Gradient instead of surface normals!
    vec3 N = estimateNormal(point);
	//Light vector (vector from fragment to light).
    vec3 L = lightPos - point;
    float dist = length(L);
    L /= dist;
	//View vector (vector from fragment to camera).
    vec3 V = normalize(eye - point);
	//Reflection vector (reflect the vector from the light to the fragment about the surface normal, resulting in a vector from the resultant ray ).
    vec3 R = normalize(reflect(-L, N));
    
	//Diffuse.
    float dotLN = dot(L, N);
	//Specular.
    float dotRV = dot(R, V);
    //Falloff.
    float attenuation = 1.0 / (1.0 + dist * 0.01 + dist * dist * 0.001);
    
    //Sum the influence of each component.
    return diffuse * max(dotLN, 0.0) + specular * pow(max(dotRV, 0.0), alpha) * attenuation;
}

const vec3 ambient = vec3(0.2);
//Eventually send uniforms for things like ambient light, and per-source diffuse and specular.
vec3 phongIllumination(vec3 diffuse, vec3 specular, float alpha, vec3 point, vec3 eye) {
    vec3 colour = ambient;
    
    vec3 light1Pos = vec3(4.0 * sin(u_time), 2.0, 4.0 * cos(u_time));
    colour += phongPointLight(diffuse, specular, alpha, point, eye, light1Pos);
    
    vec3 light2Pos = vec3(2.0 * sin(0.33 * u_time), 2.0 * cos(0.33 * u_time), 2.0);
    colour += phongPointLight(diffuse, specular, alpha, point, eye, light2Pos);

    return colour;
}

//Grahm-Schmidt technique to use the cross product to solve for right, forward, and above given the look direction and up vector.
mat4 viewMatrix(vec3 eye, vec3 center, vec3 up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat4(
        vec4(s, 0.0),
        vec4(u, 0.0),
        vec4(-f, 0.0),
        vec4(0.0, 0.0, 0.0, 1)
    );
}

void main() {
	vec3 dir = rayDirection(45.0, u_resolution, gl_FragCoord.xy);
    vec3 eye = vec3(0.0, 0.0, 5.0);
    float dist = marchScene(eye, dir, MIN_DIST, MAX_DIST);

	if (dist > MAX_DIST - EPSILON) {
        //Make this equivalent to glClearColor() eventually via uniform vector.
        outColour = vec4(0.0, 0.0, 0.0, 1.0);
		return;
    }

    //The closest point on the surface to the eyepoint along the view ray.
    vec3 point = eye + dist * dir;

    vec3 diffuse = vec3(0.7, 0.2, 0.2);
    vec3 specular = vec3(0.2);
    float specularStrength = 16.0;

    vec3 colour = phongIllumination(diffuse, specular, specularStrength, point, eye);
    
    outColour = vec4(colour, 1.0);
}