#version 330 core

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 uv;

out vec2 texCoords;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    texCoords = uv;
}