#version 330

uniform sampler2D textureMap0;

in vec2 iTexCoord0;

out vec4 pixelColor; //Output variable. Almost final pixel color.

void main(void) {
	vec4 kd = texture(textureMap0, iTexCoord0);
	pixelColor = kd;
}