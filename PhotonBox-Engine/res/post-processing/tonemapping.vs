#version 120 core

attribute vec2 position;

varying vec2 texCoordVarying;

void main(){
	gl_Position = vec4(position, 0, 1.0);
	texCoordVarying = (position.xy + vec2(1)) * 0.5;
}