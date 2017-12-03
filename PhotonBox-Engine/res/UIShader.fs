#version 120 core

uniform sampler2D diffuse;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = texture2D(diffuse, texCoordVarying)* ambient;
}