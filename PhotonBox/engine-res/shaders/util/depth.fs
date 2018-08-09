#version 330

in vec2 texCoords;

uniform sampler2D albedoMap;

void main()
{          
	if(texture(albedoMap, texCoords).a < 0.1) discard;
}  