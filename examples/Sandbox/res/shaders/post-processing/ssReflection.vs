/*
Based on the ssreflections shader from http://imanolfotia.com/blog/update/2017/03/11/ScreenSpaceReflections.html
*/

#version 330 core

layout (location = 0) in vec3 position;

out vec2 TexCoords;

void main(){
	gl_Position = vec4(position.xy, 0, 1.0);
	TexCoords = (position.xy + vec2(1)) * 0.5;
}
