#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gMetallic;
layout (location = 3) out vec4 gRoughness;
//layout (location = 4) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

//uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;

void main()
{    
    gPosition = vec4(FragPos, 1);
    gNormal = vec4(TBN * (255.0/128.0 * texture(normalMap, TexCoords).rgb - 1), 1);
	gMetallic = texture(metallicMap, TexCoords).rgba;
	gRoughness = texture(roughnessMap, TexCoords).rgba;
//	gAlbedo = texture(albedoMap, TexCoords).rgb;
}