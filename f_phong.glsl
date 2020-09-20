#version 330
#define NR_POINTS 2

uniform sampler2D textureMap0;

in vec4 observer;
in vec4 norm;
in vec4 lightPosDir[NR_POINTS];
in vec2 iTexCoord0;


out vec4 pixelColor; //Output variable. Almost final pixel color.

vec3 calcPointLight(vec4 lightPosDir, vec3 lightColor, vec3 kd, vec4 observerDir) {
	// ambient
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec4 normal = normalize(norm);
	float diffuseStrength = clamp(dot(norm, lightPosDir), 0.0, 1.0);
	vec3 diffuse = diffuseStrength * lightColor;
	
	// specular
	vec4 reflectedDir = reflect(-lightPosDir, norm);
	vec4 lightDir = normalize(lightPosDir);
	float spec = pow(clamp(dot(observerDir, reflectedDir), 0.0, 1.0), 32);
	vec3 specular = spec * lightColor;

	return kd * ambient + kd * diffuse + lightColor * specular;
}

void main(void) {
	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 nNorm = normalize(norm);
	vec4 observerDir = normalize(observer);

	//Surface parameters
	vec4 kd = texture(textureMap0, iTexCoord0); // material color
	vec4 ks = vec4(1, 1, 1, 1); // color of light source
	
	vec3 result = vec3(0.0f);
	for (int i = 0; i < NR_POINTS; i++) {
		result += calcPointLight(lightPosDir[i], lightColor.xyz, kd.xyz, observerDir);
	}

	pixelColor = vec4(result, 1.0f);
}