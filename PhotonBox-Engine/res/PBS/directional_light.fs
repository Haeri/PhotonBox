#version 120

struct DirectionalLight{
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirectionalLight light;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;

uniform vec3 viewPos;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying mat3 tbnVarying;

const float PI = 3.14159265359;
const float F0_DEFAULT = 0.04;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main(){
    vec3 albedo     = pow(texture2D(albedoMap, texCoordVarying).rgb, vec3(2.2));
    vec3 normal     = tbnVarying * (255.0/128.0 * texture2D(normalMap, texCoordVarying).rgb - 1);
    float metallic  = texture2D(metallicMap, texCoordVarying).r;
    float roughness = texture2D(roughnessMap, texCoordVarying).r;

    vec3 F0 = vec3(F0_DEFAULT); 
    F0 = mix(F0, albedo, metallic);

    vec3 radiance = light.color * light.intensity;

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - positionVarying);

    // calculate per-light radiance
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
    vec3 specular     = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;     
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    vec3 color = (kD * albedo / PI + specular) * radiance * NdotL;


    // Temporary gamma correction
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    gl_FragColor = vec4(color, 1.0);
}
  

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

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}