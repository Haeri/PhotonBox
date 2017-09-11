#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 mvp;
uniform mat4 modelMatrix;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
	positionVarying = (modelMatrix * vec4(position, 1.0)).xyz;
	normalVarying = (modelMatrix * vec4(normal, 0.0)).xyz;	
	texCoordVarying = uv;
}