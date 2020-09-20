#version 330

uniform sampler2D textureMap0;

in vec4 observer;
in vec4 norm;
in vec4 lightPos;
in vec2 iTexCoord0;

out vec4 pixelColor; //Output variable. Almost final pixel color.

void main(void) {
	vec4 lightColor = vec4(1, 1, 1, 1);

	vec4 lightPosDir = normalize(lightPos);
	vec4 nNorm = normalize(norm);
	vec4 observerDir = normalize(observer);

	//Surface parameters
	vec4 kd = texture(textureMap0, iTexCoord0); // material color
	vec4 ks = vec4(1, 1, 1, 1); // color of light source

	// ambient
	float ambientStrength = 0.4;
	vec4 ambient = ambientStrength * lightColor;

	// diffuse
	vec4 normal = normalize(norm);
	vec4 lightDir = normalize(lightPos);
	float diffuseStrength = clamp(dot(norm, lightDir), 0.0, 1.0);
	vec4 diffuse = diffuseStrength * lightColor;
	
	// specular
	vec4 reflectedDir = reflect(-lightPosDir, nNorm);
	float spec = pow(clamp(dot(observerDir, reflectedDir), 0.0, 1.0), 32);
	vec4 specular = spec * lightColor;
	
	vec4 am = vec4(kd.rgb * ambient.rgb, kd.a);
	vec4 dif = vec4(kd.rgb * diffuse.rgb, kd.a);
	vec4 spe = vec4(lightColor.rgb * specular.rgb, kd.a);
	pixelColor = am + dif + spe;
}