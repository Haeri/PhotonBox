#version 120

uniform sampler2D albedoMap;
uniform float boost;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_FragColor = texture2D(albedoMap, texCoordVarying) * boost;
}
