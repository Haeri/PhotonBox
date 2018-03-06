/*
Based on the prefilter shader from https://github.com/JoeyDeVries/LearnOpenGL
*/

#version 330

attribute vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;

void main()
{
    WorldPos = position;  
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
