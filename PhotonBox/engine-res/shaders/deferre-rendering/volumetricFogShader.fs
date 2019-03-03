#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

const float M_PI = 3.141592653589793238;

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

// CONSTANTS
const int MAX_DIRECTIONAL_LIGHTS = 3;
const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

// UNIFORMS
uniform sampler2D gPosition;

uniform sampler2D shadowMap;
uniform sampler2D noise;

uniform mat4 viewMatrixInv;
uniform mat4 projectionMatrixInv;
uniform int numDirectionalLights;
uniform int numPointLights;
uniform int numSpotLights;;

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];



// Absorption specific parameters
float absorptionTau = 0.02;
vec3 absorptionColor = vec3(0.5, 0.5, 0.5);
float raySamples = 20.0;

// Scattering specific parameters
float scatteringTau = 0.007;
vec3 scatteringColor = vec3(1, 1, 1);



vec3 fragPos;

vec4 volumetricShadows();
float random(vec2 co);

void main()
{
    fragPos = textureLod(gPosition, TexCoords, 0).rgb;
    FragColor = volumetricShadows();
}

vec3 absorptionTransmittance(float dist) {
    return absorptionColor*exp(-dist*(absorptionTau+scatteringTau));
}

/**
 * Rayleigh scattering phase function
 */
float phaseFunctionRayleigh(vec3 inDir, vec3 outDir) {
    float cosAngle = dot(inDir, outDir) / (length(inDir) * length(outDir));
    float nom = 3.0*(1.0 + cosAngle*cosAngle);
    float denom = 16.0 * M_PI;
    return nom/denom;
}


vec4 volumetricShadows()
{
	
	float density = 0.01;

    float fragDistance;

    if(fragPos.z == 0)
       fragDistance = 1000;
    else
	   fragDistance = -fragPos.z;

	float delta = fragDistance / (raySamples+1);	   

    vec4 view_space_ray_4 = projectionMatrixInv * vec4(TexCoords.x*2-1, TexCoords.y*2-1, 1, 1);
    vec3 view_space_ray = normalize(view_space_ray_4.xyz / view_space_ray_4.w);

    float rand = random(fragPos.xy+fragPos.z);
    vec3 ray = (view_space_ray * delta * rand);
	vec3 ret;

	vec4 world_space_ray_4 = viewMatrixInv * view_space_ray_4;
	vec3 world_space_ray = normalize(world_space_ray_4.xyz / world_space_ray_4.w);
	
	 vec3 fragToCamNorm = normalize(world_space_ray);

	for(int i = 0; i < raySamples; ++i){
        ray += (view_space_ray * delta);
    	vec4 fragPosLightSpace = directionalLights[0].lightSpaceMatrix * viewMatrixInv * vec4(ray, 1);
 		vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        float closestDepth = texture2D(shadowMap, projCoords.xy).r; 
    	float currentDepth = projCoords.z;

		float visibility = closestDepth > currentDepth ? 1 : 0;
        //vec3 lightToX = x - light.position;
        //float lightDist = length(lightToX);
        float lightDist = 0.07;
        float omega = 4 * M_PI * lightDist * lightDist;
        vec3 Lin = absorptionTransmittance(lightDist) * visibility * directionalLights[0].color * directionalLights[0].intensity / omega;
        vec3 Li = Lin * scatteringTau * scatteringColor * phaseFunctionRayleigh(normalize(directionalLights[0].direction), fragToCamNorm);
        ret += Li * absorptionTransmittance(length(ray)) * length(delta);
        //x += deltaStep;

	}
	return vec4(ret, 1);
}

float random(vec2 co) {
    //return texture(noise, co).r *2 -1;
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}