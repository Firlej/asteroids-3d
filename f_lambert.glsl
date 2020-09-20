#version 330
#define NR_POINTS 2

uniform sampler2D textureMap0;

in vec4 norm;
in vec2 iTexCoord0;
in vec4 lightPosDir[NR_POINTS];

out vec4 pixelColor; //Output variable. Almost final pixel color.

vec3 calcPointLight(vec4 lightPos, vec3 lightColor, vec3 kd) {
	// ambient
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec4 normal = normalize(norm);
	vec4 lightDir = normalize(lightPos);
	float diffuseStrength = clamp(dot(norm, lightDir), 0.0, 1.0);
	vec3 diffuse = diffuseStrength * lightColor;
	
	// combine
	return ambient.rgb * kd.rgb + diffuse.rgb * kd.rgb;
}

void main(void) {

	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 kd = texture(textureMap0, iTexCoord0); // material color

	vec3 result = vec3(0.0f);
	for (int i = 0; i < NR_POINTS; i++) {
		result += calcPointLight(lightPosDir[i], lightColor.rgb, kd.rgb);
	}

	pixelColor = vec4(result, kd.a);
}

