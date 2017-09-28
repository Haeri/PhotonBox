#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 transform;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	gl_Position = transform * vec4(position, 1.0);
	texCoordVarying = uv;
	normalVarying = (transform * vec4(normal, 0.0)).xyz;	
}