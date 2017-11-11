#version 330

attribute vec3 position;

uniform mat4 projection;
uniform mat4 view;

varying vec3 WorldPos;

void main()
{
    WorldPos = position;  
    gl_Position =  projection * view * vec4(WorldPos, 1.0);

    //gl_Position = vec4(position, 1);
}