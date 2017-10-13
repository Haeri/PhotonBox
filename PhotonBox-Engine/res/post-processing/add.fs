#version 120

uniform sampler2D renderTexture;
uniform sampler2D renderTexture2;

varying vec2 texCoordVarying;

void main(){
	//gl_FragColor = texture2D(renderTexture2, texCoordVarying);
	gl_FragColor = texture2D(renderTexture, texCoordVarying) + texture2D(renderTexture2, texCoordVarying);
}