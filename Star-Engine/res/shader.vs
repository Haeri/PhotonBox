#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec3 color;
attribute vec2 uv;

uniform mat4 transform;

varying vec2 texCoord0;

void main(){
	gl_Position = transform * vec4(position, 1.0);
	texCoord0 = uv;
}