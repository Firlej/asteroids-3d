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
out vec4 observer;
out vec4 norm;
out vec4 lightPos;

//Varying variables

void main(void) {
    vec4 lp = vec4(-100, 100, 500, 1); // place of light
    lightPos = normalize(V * lp - V * M * vertex); // vector towards light source in eye space
    observer = normalize(vec4(0, 0, 0, 1) - V * M * vertex); // vector towards observer in eye space
    norm = normalize(V * M * normal); // normal vector in eye space

    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}