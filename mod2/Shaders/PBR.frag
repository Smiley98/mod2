#version 440

in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec3 posEye;
} vIn;

out vec4 outColour;

uniform vec3 u_light1PosView;



//Li is the radiance at point P from a ray in the direction Wi.
//NdotWi scales the radiance based on the incident angle to the surface.
//Lo(P, Wo) is the reflected sum of all light's irradiance onto point P as viewed from Wo.

//Lo(P,Wo) = ∫ Fr(P, Wi, Wo) Li(P, Wi)N⋅Wi(dWi)
///*//Reflectance equation translated to code.
float dW  = 1.0f / steps;
//Integrals are sums of area.
//More steps means more precise sum.
//We must scale the area by dW in order to prevent additional steps from going outside the area.
for(int i = 0; i < steps; i++) {
    vec3 Wi = getNextIncomingLightDir(i);
    sum += Fr(P, Wi, Wo) * Li(P, Wi) * dot(N, Wi) * dW;
}//*/

void main() {
    vec3 normal = normalize(vIn.normal);

}