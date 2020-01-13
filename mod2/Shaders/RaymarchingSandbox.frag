#version 440

//Interface block (not applicable what so ever to raymarching).
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

//Output colour.
out vec4 outColour;

//Limit load from raymarching.
const int MAX_MARCHING_STEPS = 255;

//Extremely small value to ensure our comparisons work the way we want due to floating point precision.
const float EPSILON = 0.0001;

//Model transforms.
uniform mat4 u_modelTransform;

//Camera transform (NOT view [inverse camera] transform) spread between rotation and translation. 
uniform mat3 u_cameraRotation;
uniform vec3 u_cameraTranslation;

//Projection uniforms.
uniform vec2 u_resolution;
uniform float u_projectionDistance;
uniform float u_nearPlane;
uniform float u_farPlane;

//Utility uniforms.
uniform float u_time;
uniform float u_period;

float intersectSDF(float a, float b) {
    return max(a, b);
}

float unionSDF(float a, float b) {
    return min(a, b);
}

float differenceSDF(float a, float b) {
    return max(a, -b);
}

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

mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

//Function describing all the scene geometry (currently just one circle with a radius of one about the orgin).
float sceneSDF(vec3 point) {
    mat4 transformationMatrix = u_modelTransform;
    vec4 point4 = vec4(point, 1.0);
    vec3 cube1 = (transformationMatrix * point4).xyz;
    transformationMatrix[3] = vec4(-2.0, 0.0, 0.0, 1.0);
    vec3 cube2 = (transformationMatrix * point4).xyz;

    float nearestPoint = cubeSDF(cube1, 1.0);
    nearestPoint = unionSDF(nearestPoint, cubeSDF(cube2, 1.0));
    return nearestPoint;
}

//Distance between the ray and surface geometry. Very fundamental.
float marchScene(vec3 eye, vec3 rayDirection, float near, float far) {
    //For geometry before the near plane, the rays will exceed them until either the far plane is hit or the step limit is reached.
    float depth = near;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        //The first iteration always takes us from the eye to the image plane because depth is initially 0. This 0 value is multiplied by the ray direction, effectively removing it for the iteration.
        float rayStep = sceneSDF(eye + depth * rayDirection);
        //(Change this to 0 for a dissolve effect. This is because not every point is exactly inside the surface geometry due to precision.
        if (rayStep < EPSILON) {
            //Return the cumulative distance if the ray intersects with geometry (SDF evalutates to a negative number [distance less than epsilon]).
			return depth - near;
        }
        depth += rayStep;
        //Return if the ray exceeds the far clipping plane. (Start is the near plane. Works the way you'd think. If 50, would discard geometry less than 50 units away from camera by always starting 50 units ahead).
        if (depth >= far) {
            return far;
        }
    }
    //Return how far the ray travelled if it didn't intersect with geometry or exceed the far plane before capping out on steps.
    return far;
}

//Returns a (ray) direction based on the proportionality between the fragment and projection plane.
//(Normalized vector from triangulated centre of the screen to fragment [from ray origin to image plane]).
vec3 rayDirection(vec2 screenSize, vec2 fragCoord) {
    vec2 xy = fragCoord - screenSize * 0.5;
    return normalize(vec3(xy, u_projectionDistance));
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

void main() {
    //Shoot a ray from world origin - distance to projection plane towards the current fragment.
	vec3 rayWorldSpace = rayDirection(u_resolution, gl_FragCoord.xy);
    //Apply the camera's rotation to the ray.
    vec3 rayCameraSpace = vec3(u_cameraRotation * rayWorldSpace);

    float intersectionDistance = marchScene(u_cameraTranslation, rayCameraSpace, u_nearPlane, u_farPlane);

    //Colour background and discard if the ray intersection test failed (either exceeded the far plane or number of steps).
	if (intersectionDistance > u_farPlane - EPSILON) {
        outColour = vec4(0.0, 0.0, 0.0, 1.0);
		return;
    }

    //The closest point on the surface to the eyepoint along the view ray.
    vec3 poi = u_cameraTranslation + intersectionDistance * rayCameraSpace;

    vec3 diffuse = vec3(0.7, 0.2, 0.2);
    vec3 specular = vec3(0.2);
    float specularStrength = 16.0;

    vec3 colour = phongIllumination(diffuse, specular, specularStrength, poi, u_cameraTranslation);
    
    outColour = vec4(colour, 1.0);
}
