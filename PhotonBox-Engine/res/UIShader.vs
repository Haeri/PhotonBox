#version 120 core

attribute vec3 position;
attribute vec3 color;
attribute vec2 uv;

varying vec2 texCoordVarying;

void main(){
	gl_Position = vec4(position, 1.0);
	texCoordVarying = uv;
}