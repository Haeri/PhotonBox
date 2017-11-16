#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;

uniform vec3 viewPos;

uniform samplerCube irradianceMap;
uniform samplerCube convolutedSpecularMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;

varying vec3 positionVarying;
varying vec2 texCoordVarying;
varying mat3 tbnVarying;

const float F0_DEFAULT = 0.04;
const float MAX_REFLECTION_LOD = 4.0;

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void main(){
    vec3 albedo     = pow(texture2D(albedoMap, texCoordVarying).rgb, vec3(2.2));
    vec3 emission   = texture2D(emissionMap, texCoordVarying).rgb;
    vec3 normal     = tbnVarying * (255.0/128.0 * texture2D(normalMap, texCoordVarying).rgb - 1);
    float ao        = texture2D(aoMap, texCoordVarying).r;
    float metallic  = texture2D(metallicMap, texCoordVarying).r;
    float roughness = texture2D(roughnessMap, texCoordVarying).r;
	
	vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - positionVarying);
	vec3 R = reflect(-V, N);
	
    vec3 irradiance	= textureCube(irradianceMap, N).rgb;
   	vec3 convolutedSpecular = textureCubeLod(convolutedSpecularMap, R, roughness * MAX_REFLECTION_LOD).rgb;

    vec3 ambientLight = light.color * light.intensity;

    vec3 F0 = vec3(F0_DEFAULT); 
    F0 = mix(F0, albedo, metallic);

	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	vec3 diffuse  = irradiance * albedo;
	vec3 specular = convolutedSpecular * (F); // * brdf.x + brdf.y);
	vec3 ambient  = (kD * diffuse + specular) * ao; 

	vec3 color = ambient + ambientLight + emission;

    // Temporary gamma correction
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    gl_FragColor = vec4(color, 1.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  