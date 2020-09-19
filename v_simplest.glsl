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
    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}
