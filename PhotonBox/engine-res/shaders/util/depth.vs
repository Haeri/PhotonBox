#version 120

attribute vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

varying vec2 TexCoords;

void main()
{
	TexCoords = position.xy;
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0);
}  
