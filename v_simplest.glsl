#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
in vec4 vertex;
in vec2 texCoord0;

out vec2 iTexCoord0;

//Varying variables

void main(void) {
    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}
