#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct GData{
    vec3 Position;
    vec3 Normal;
    vec3 Albedo;
    vec3 Emission;
    float Roughness;
    float Metallic;
    float Occlusion;
};

struct Attenuation{
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    
    Attenuation attenuation;
};

struct DirectionalLight{
    vec3 direction;
    vec3 color;

    mat4 lightSpaceMatrix;

    float intensity;
};

GData gData;

// CONSTANTS
const int NR_LIGHTS = 3;
const float PI = 3.14159265359;
const float F0_DEFAULT = 0.04;


// UNIFORMS
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gRoughness;
uniform sampler2D gMetallic;
uniform sampler2D gAlbedo;

uniform sampler2D shadowMap;
uniform mat4 viewMatrixInv;

uniform DirectionalLight directionalLights[NR_LIGHTS];
//uniform vec3 viewPos;



float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

vec3 DirectionalLightBRDF(DirectionalLight directionalLight);
vec3 PointLightBRDF();
vec3 SpotLightBRDF();

void main()
{             
    if(texture(gPosition, TexCoords).xyz == vec3(0, 0, 0)){
    	discard;
    }

    // retrieve data from gbuffer
    gData.Position = texture(gPosition, TexCoords).rgb;
	if(gData.Position == vec3(0, 0, 0)) discard;
    gData.Normal = texture(gNormal, TexCoords).rgb;
    gData.Albedo = texture(gAlbedo, TexCoords).rgb;
    gData.Roughness = texture(gRoughness, TexCoords).r;
    gData.Metallic = texture(gMetallic, TexCoords).r;
    
    vec3 finalColor;

    for(int i = 0; i < NR_LIGHTS; ++i){
        finalColor += DirectionalLightBRDF(directionalLights[i]);
    }
  
    FragColor = vec4(finalColor, 1);
}



vec3 DirectionalLightBRDF(DirectionalLight directionalLight){
    vec3 F0 = vec3(F0_DEFAULT); 
    F0 = mix(F0, gData.Albedo, gData.Metallic);

    vec3 radiance = directionalLight.color * directionalLight.intensity;

    vec3 N = normalize(gData.Normal);
    vec3 V = normalize(-gData.Position);

    // calculate per-light radiance
    vec3 L = normalize(-directionalLight.direction);
    vec3 H = normalize(V + L);
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, gData.Roughness);        
    float G   = GeometrySmith(N, V, L, gData.Roughness);  
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
    vec3 specular     = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - gData.Metallic;     

    // shadows
    vec4 fragPosLightSpace = directionalLight.lightSpaceMatrix * viewMatrixInv * vec4(gData.Position, 1);
    float shadow = ShadowCalculation(fragPosLightSpace, N, L);     
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    vec3 color = ((kD * gData.Albedo) / PI + specular) * radiance * (1.0 - shadow) * NdotL;

    return color;
}








// ---------------- Components ---------------- //

float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    //float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001); 
    float bias = 0.0001;

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
  //  float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;



    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;


    return shadow;
}  
