#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gMetallic;
layout (location = 3) out vec3 gRoughness;
layout (location = 4) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;

void main()
{    
    gPosition = FragPos;
    gNormal = TBN * (255.0/128.0 * texture(normalMap, TexCoords).rgb - 1);
	gMetallic = texture(metallicMap, TexCoords).rgb;
	gRoughness = texture(roughnessMap, TexCoords).rgb;
	gAlbedo = texture(albedoMap, TexCoords).rgb;
}