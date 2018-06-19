#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gMetallic;
layout (location = 3) out vec4 gRoughness;
layout (location = 4) out vec4 gAlbedo;
layout (location = 5) out vec4 gEmission;
//layout (location = 6) out vec4 gAO;
layout (location = 6) out vec4 gIrradiance;
layout (location = 7) out vec4 gRadiance;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Pos;
in mat3 TBN;

uniform mat4 view;
uniform vec3 viewPos;
uniform vec3 minBound;
uniform vec3 maxBound;
uniform vec3 boundPos;
uniform int useCorrection = 0;

uniform samplerCube irradianceMap;
uniform samplerCube convolutedSpecularMap;

uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;
uniform sampler2D emissionMap;
//uniform sampler2D aoMap;

const float MAX_REFLECTION_LOD = 4.0;

vec3 correctedCubeMapDir(vec3 v, vec3 pos);
float correctedCubeMapWeight(vec3 v, vec3 pos);

void main()
{    
	if(texture(albedoMap, TexCoords).a < 0.1) discard;
	
    gPosition = vec4(FragPos, 1);
    vec4 normal = vec4(TBN * (255.0/128.0 * texture(normalMap, TexCoords).rgb - 1), 0);
    gNormal = view * normal;
	gMetallic = texture(metallicMap, TexCoords).rgba;
	gRoughness = texture(roughnessMap, TexCoords).rgba;
	vec4 alb = texture(albedoMap, TexCoords).rgba;
	gAlbedo = vec4(pow(alb.xyz, vec3(2.2)), alb.a);

	gEmission = texture(emissionMap, TexCoords).rgba;
	//gAO = texture(aoMap, TexCoords).rgba;




	float roughness = texture(roughnessMap, TexCoords).r;

	vec3 N = normalize(normal.xyz);
	vec3 V = normalize(viewPos - Pos);
	vec3 R = reflect(-V, N);

	vec3 cN = N;
    if(useCorrection > 0.5){
        float distance = correctedCubeMapWeight(N, Pos); 
        cN = correctedCubeMapDir(N, Pos);
    }
   	vec3 irradiance = texture(irradianceMap, cN).rgb;

    vec3 cR = R;
    if(useCorrection > 0.5)
        cR = correctedCubeMapDir(R, Pos);
   	vec3 convolutedSpecular = textureLod(convolutedSpecularMap, cR, roughness * MAX_REFLECTION_LOD).rgb;



   	gIrradiance = vec4(irradiance, 1);
  	gRadiance = vec4(convolutedSpecular, 1);
}





vec3 correctedCubeMapDir(vec3 v, vec3 pos){
    // Normalize directionvector
    vec3 nDir = normalize(v);

    // Figure out the correction factor for every component
    vec3 rbmax = (maxBound - pos)/nDir;
	vec3 rbmin = (minBound - pos)/nDir;

    vec3 rbminmax;
    rbminmax.x = ( nDir.x > 0.0 )?rbmax.x:rbmin.x;
    rbminmax.y = ( nDir.y > 0.0 )?rbmax.y:rbmin.y;
    rbminmax.z = ( nDir.z > 0.0 )?rbmax.z:rbmin.z;

    float correction = min(min(rbminmax.x, rbminmax.y), rbminmax.z);

    // apply the appropriate correction according to the quadrant to the resulting vector
    vec3 boxIntersection = pos + nDir * correction;

    return boxIntersection - boundPos;
}

float correctedCubeMapWeight(vec3 v, vec3 pos){
    // Normalize directionvector
    vec3 nDir = normalize(v);

    // Figure out the correction factor for every component
    vec3 rbmax = (maxBound - pos)/nDir;
    vec3 rbmin = (minBound - pos)/nDir;

    vec3 rbminmax;
    rbminmax.x = ( nDir.x > 0.0 )?rbmax.x:rbmin.x;
    rbminmax.y = ( nDir.y > 0.0 )?rbmax.y:rbmin.y;
    rbminmax.z = ( nDir.z > 0.0 )?rbmax.z:rbmin.z;

    return min(min(rbminmax.x, rbminmax.y), rbminmax.z);
}
