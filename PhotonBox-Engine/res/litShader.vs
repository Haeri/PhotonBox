#version 120

attribute vec3 position;

uniform mat4 mvp;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
}