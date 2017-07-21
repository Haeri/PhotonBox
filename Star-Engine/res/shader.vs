#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec3 color;
attribute vec2 uv;

varying vec3 color_varying;

void main(){
	gl_Position = vec4(position, 1.0);
	color_varying = color;
}