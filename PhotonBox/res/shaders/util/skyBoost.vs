#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 mvp;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
	texCoordVarying = uv;
	normalVarying = (mvp * vec4(normal, 0.0)).xyz;	
}
