/*
Based on the irradiance_convolution shader from https://github.com/JoeyDeVries/LearnOpenGL
*/

#version 120

attribute vec3 position;

uniform mat4 projection;
uniform mat4 view;

varying vec3 WorldPos;

void main()
{
    WorldPos = position;  
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
