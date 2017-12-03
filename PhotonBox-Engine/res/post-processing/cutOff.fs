#version 120 core

uniform sampler2D renderTexture;
uniform float threshold;

varying vec2 texCoordVarying;

void main(){
	vec4 color = texture2D(renderTexture, texCoordVarying);
	float luma = (0.2126 * color.x + 0.7152 * color.y + 0.0722 * color.z);
	if(luma >= threshold){
		gl_FragColor = color;
	}else{
		gl_FragColor = vec4(0, 0, 0, 1);
	}
}