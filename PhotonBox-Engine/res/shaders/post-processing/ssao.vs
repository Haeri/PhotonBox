/*
Based on the ssao shader from https://github.com/JoeyDeVries/LearnOpenGL
*/

#version 330
layout (location = 0) in vec3 position;

out vec2 TexCoords;

void main(){
	gl_Position = vec4(position.xy, 0, 1.0);
	TexCoords = (position.xy + vec2(1)) * 0.5;
}
