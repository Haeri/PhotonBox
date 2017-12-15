#version 120

uniform sampler2D diffuse;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = texture2D(diffuse, texCoordVarying)* ambient;
}
