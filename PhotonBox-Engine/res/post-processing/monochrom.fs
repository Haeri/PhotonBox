#version 120 core

uniform sampler2D renderTexture;

varying vec2 texCoordVarying;

void main(){
	vec4 color = texture2D(renderTexture, texCoordVarying);
	float bw = 0.21 * color.x +  0.72 * color.y + 0.07 * color.z;
	gl_FragColor = vec4(bw, bw, bw, 1);
}