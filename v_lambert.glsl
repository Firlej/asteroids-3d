#version 330
#define NR_POINTS 2

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec3 lightPositions[NR_POINTS];

//Attributes
in vec4 vertex;
in vec4 normal;
in vec2 texCoord0;

out vec2 iTexCoord0;
out vec4 norm;
out vec4 lightPosDir[NR_POINTS];


void main(void) {
	norm = normalize(V * M * normal); // normal vector in eye space
	iTexCoord0 = texCoord0;
	for (int i = 0; i < NR_POINTS; i++) {
		lightPosDir[i] = normalize(V * vec4(lightPositions[i], 1) - V * M * vertex);
	}
    gl_Position = P * V * M * vertex;
}