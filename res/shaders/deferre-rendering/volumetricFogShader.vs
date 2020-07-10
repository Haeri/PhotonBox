#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vec3(aPos.xy, 0), 1.0);
	TexCoords = (aPos.xy + vec2(1)) * 0.5;
}