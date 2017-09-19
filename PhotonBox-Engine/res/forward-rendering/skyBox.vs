#version 120

attribute vec3 position;

uniform mat4 vp;

varying vec3 texCoordVarying;

void main(){
	gl_Position = vp * vec4(position, 1.0);
	texCoordVarying = position;
}