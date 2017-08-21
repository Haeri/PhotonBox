#version 120

uniform sampler2D diffuse;

uniform vec4 lightColor;
uniform float lightIntensity;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_FragColor = vec4(lightIntensity * lightColor.xyz, 1) * texture2D(diffuse, texCoordVarying);
}