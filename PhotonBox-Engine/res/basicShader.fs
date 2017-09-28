#version 120

uniform sampler2D diffuseMap;

vec4 ambient = vec4(0.4f, 0.4f, 0.4f, 1);

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_FragColor = texture2D(diffuseMap, texCoordVarying)* ambient;
}