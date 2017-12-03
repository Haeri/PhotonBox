#version 120 core

uniform sampler2D renderTexture;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = texture2D(renderTexture, texCoordVarying);
}