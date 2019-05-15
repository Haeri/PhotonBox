#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct GData{
    vec3 Position;
    vec3 Normal;
    vec3 Albedo;
    vec3 Emission;
    vec3 Irradiance;
    vec3 Radiance;

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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float coneAngle;
    float coneFallOff;
    
    vec3 color;
    float intensity;
    
    Attenuation attenuation;
};

GData gData;

// CONSTANTS
const int MAX_DIRECTIONAL_LIGHTS = 3;
const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;


const float PI = 3.14159265359;
const float F0_DEFAULT = 0.04;
const float AMBIENT_ATTENUATION = 0.1;


// UNIFORMS
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gRoughness;
uniform sampler2D gMetallic;
uniform sampler2D gAlbedo;
uniform sampler2D gIrradiance;
uniform sampler2D gRadiance;
uniform sampler2D gEmission;

uniform sampler2D shadowMap;
uniform sampler2D noise;

uniform mat4 viewMatrixInv;
uniform int numDirectionalLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform mat4 projection;

float stepScalar = 0.01;

bool useContactShadows = true;


uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec3 F0;
vec3 N;
vec3 V;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

vec3 BasePass();
vec3 DirectionalLightBRDF(DirectionalLight directionalLight);
vec3 PointLightBRDF(PointLight pointLight);
vec3 SpotLightBRDF(SpotLight spotLight);

int rayMarch(vec3 dir);
float random(vec2 co);

void main()
{
    // retrieve data from gbuffer
    gData.Position = textureLod(gPosition, TexCoords, 0).rgb;
	if(gData.Position == vec3(0, 0, 0)) discard;
    gData.Normal = textureLod(gNormal, TexCoords, 0).rgb;
    gData.Albedo = texture(gAlbedo, TexCoords).rgb;
    gData.Roughness = texture(gRoughness, TexCoords).r;
    gData.Metallic = texture(gMetallic, TexCoords).r;
    gData.Irradiance = texture(gIrradiance, TexCoords).rgb;
    gData.Radiance = texture(gRadiance, TexCoords).rgb;
    gData.Emission = texture(gEmission, TexCoords).rgb;


    F0 = mix(vec3(F0_DEFAULT), gData.Albedo, gData.Metallic);
    N = normalize(gData.Normal);
    V = normalize(-gData.Position);
    
    vec3 finalColor = BasePass();

    for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
        if(numDirectionalLights == i) 
            break;
        finalColor += DirectionalLightBRDF(directionalLights[i]);
    }
    
    for(int i = 0; i < MAX_POINT_LIGHTS; ++i){
        if(numPointLights == i) 
            break;
        finalColor += PointLightBRDF(pointLights[i]);
    }

    for(int i = 0; i < MAX_SPOT_LIGHTS; ++i){
        if(numSpotLights == i) 
            break;
        finalColor += SpotLightBRDF(spotLights[i]);
    }

    FragColor = vec4(finalColor, texture(gAlbedo, TexCoords).a);
}


vec3 BasePass(){
    //vec3 ambientLight = light.color * light.intensity;

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, gData.Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - gData.Metallic;

    vec3 diffuse  = gData.Irradiance * gData.Albedo;
    vec3 specular = gData.Radiance * (F); // * brdf.x + brdf.y);
    vec3 ambient  = (kD * diffuse + specular);

    vec3 color = ambient + /*ambientLight + */gData.Emission;
   
    return color;
}

vec3 DirectionalLightBRDF(DirectionalLight directionalLight){
    vec3 L = normalize(-directionalLight.direction);
    vec3 H = normalize(V + L);

    vec3 radiance = directionalLight.color * directionalLight.intensity;    
    
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
    
    float shadow;
    if(useContactShadows)
        shadow = min(max(ShadowCalculation(fragPosLightSpace, N, L), rayMarch(L)), 1);
    else
        shadow = ShadowCalculation(fragPosLightSpace, N, L);
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    vec3 color = ((kD * gData.Albedo) / PI + specular) * radiance * (1.0 - shadow) * NdotL;

    return color;
}

vec3 PointLightBRDF(PointLight pointLight){
    vec3 L = normalize(pointLight.position - gData.Position);
    vec3 H = normalize(V + L);
    
    // attenuation
    float distance    = length(pointLight.position - gData.Position);
    float attenuation = 1.0 / (pointLight.attenuation.constant + pointLight.attenuation.linear * distance + pointLight.attenuation.quadratic * (distance * distance));     
    vec3 radiance = pointLight.color * pointLight.intensity * attenuation;

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
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    vec3 color = (kD * gData.Albedo / PI + specular) * radiance * NdotL;

    return color;
}


vec3 SpotLightBRDF(SpotLight spotLight){
	vec3 L = normalize(spotLight.position - gData.Position);
    vec3 H = normalize(V + L);
	float theta = dot(L, normalize(-spotLight.direction));

	// attenuation
	float distance    = length(spotLight.position - gData.Position);
	float attenuation = 1.0 / (spotLight.attenuation.constant + spotLight.attenuation.linear * distance + spotLight.attenuation.quadratic * (distance * distance));     

	// cone attenuation
	float epsilon   = spotLight.coneAngle - spotLight.coneFallOff;
	float coneAttenuation = clamp((theta - spotLight.coneFallOff) / epsilon, 0.0, 1.0);  

 	vec3 radiance = spotLight.color * spotLight.intensity * attenuation * coneAttenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(N, H, gData.Roughness);        
	float G   = GeometrySmith(N, V, L, gData.Roughness);      
	vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 nominator    = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
	vec3 specular     = (nominator / denominator) * coneAttenuation;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - gData.Metallic;     
	    
	// add to outgoing radiance Lo
	float NdotL = max(dot(N, L), 0.0);                
	vec3 color = (kD * gData.Albedo / PI + specular) * radiance * NdotL;

	return color;
	//return vec3(0, 0, 0);
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

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    //float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001); 
    float bias = 0.0004;

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

float rand = random(gData.Position.xy+gData.Position.z);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int x_rad = 1;
    int y_rad = 1;
    for(int x = -x_rad; x <= x_rad; ++x)
    {
        for(int y = -y_rad; y <= y_rad; ++y)
        {
            float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize * (rand*2)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= (x_rad*2+1 + y_rad*2+1);

    shadow = clamp(shadow, 0, 1);

    return shadow;
}  


int rayMarch(vec3 dir)
{
    if(-gData.Position.z > 5) return 0;

    float rand = random(gData.Position.xy+gData.Position.z);
    dir *= stepScalar;
    float max = 5;
    vec3 pos = textureLod(gPosition, TexCoords, 0).xyz;
    vec3 currpoint = pos + (dir * rand * 1);
    vec4 projectedCoord;
    float currDepth;

    for(int i = 0; i < max; i++){

        currpoint += dir;
        projectedCoord = projection * vec4(currpoint, 1.0);
        projectedCoord.xyz /= projectedCoord.w;
        projectedCoord.xyz = projectedCoord.xyz * 0.5 + 0.5;
 
        currDepth = textureLod(gPosition, projectedCoord.xy, 0).z;

        if(currDepth > currpoint.z && abs(currDepth - currpoint.z) <= 0.05  && currDepth != 0)
            return 1;
            
    }

    return 0;
}

float random(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}