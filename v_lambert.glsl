#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
in vec4 vertex;
in vec4 normal;
in vec2 texCoord0;

out vec2 iTexCoord0;
out vec4 norm;
out vec4 lightPos;


void main(void) {
	vec3 lightPosition = vec3(-100, 100, 500); // place of light
	norm = normalize(V * M * normal); // normal vector in eye space
    lightPos = normalize(V * vec4(lightPosition, 1) - V * M * vertex); // vector towards light source in eye space
	
	iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}