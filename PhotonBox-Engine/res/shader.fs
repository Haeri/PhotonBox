#version 120

uniform sampler2D diffuse;

vec4 ambient = vec4(0.4f, 0.4f, 0.4f, 1);

vec3 lightDir1 = vec3(2, 0, -4);
vec4 lightColor1 = vec4(0.5f, 0.86f, 0.96f, 1);
float intensity1 = 0.6f;

vec3 lightDir2 = vec3(-2, -1, 3);
vec4 lightColor2 = vec4(0.99f, 0.9f, 0.87f, 1);
float intensity2 = 0.7f;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_FragColor = 
	texture2D(diffuse, texCoordVarying)
	* ambient
	+
	texture2D(diffuse, texCoordVarying)
	* (intensity1 * lightColor1 * clamp(dot(-lightDir1, normalVarying), 0, 1))
	+
	texture2D(diffuse, texCoordVarying)
	* (intensity2 * lightColor2 * clamp(dot(-lightDir2, normalVarying), 0, 1));
}