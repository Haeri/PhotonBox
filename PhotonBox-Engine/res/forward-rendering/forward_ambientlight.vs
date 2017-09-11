#version 120

attribute vec3 position;
attribute vec2 uv;

uniform mat4 mvp;

varying vec2 texCoordVarying;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
	texCoordVarying = uv;
}