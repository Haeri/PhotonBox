#version 120

uniform sampler2D albedoMap;
varying vec2 TexCoords;

void main()
{          
	if(texture2D(albedoMap, TexCoords).a < 0.1) discard;
}  
