#version 330

uniform sampler2D textureMap0;

in vec4 norm;
in vec4 lightPos;
in vec2 iTexCoord0;

out vec4 pixelColor; //Output variable. Almost final pixel color.

void main(void) {

	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 kd = texture(textureMap0, iTexCoord0); // material color

	// ambient
	float ambientStrength = 0.2;
	vec4 ambient = ambientStrength * lightColor;

	// diffuse
	vec4 normal = normalize(norm);
	vec4 lightDir = normalize(lightPos);
	float diffuseStrength = clamp(dot(norm, lightDir), 0.0, 1.0);
	vec4 diffuse = diffuseStrength * lightColor;

	// combine
	vec4 result = vec4(ambient.rgb * kd.rgb, kd.a) + vec4(diffuse.rgb * kd.rgb, kd.a);
	pixelColor = result;
}