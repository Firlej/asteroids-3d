#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
in vec4 vertex;
in vec4 normal;
in vec2 texCoord0;

out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

//Varying variables

void main(void) {
    vec4 lp = vec4(0, 0, -6, 1); // place of light
    l = normalize(V * lp - V * M * vertex); // vector towards light source in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); // vector towards observer in eye space
    n = normalize(V * M * normal); // normal vector in eye space

    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}
