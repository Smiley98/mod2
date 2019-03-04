#version 440

out vec4 outColour;

float rand(vec2 n){
    return fract(sin(dot(n.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	vec2 coords = gl_FragCoord.xy;
	outColour = vec4(rand(coords), rand(vec2(800.0f) / coords) , rand(vec2(cos(coords.x), sin(coords.y))), 1.0f);
}