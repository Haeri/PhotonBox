#version 120

struct PointLight{
	vec3 position;
	vec3 color;
    float intensity;

 	float constant;
    float linear;
    float quadratic;
};


uniform PointLight light;
uniform float shininess; 
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform vec3 viewPos;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;
varying mat3 tbnVarying;

float saturate(float value){
    return max(min(value, 1), 0);
}

void main(){
  	// diffuse 
    vec3 norm = normalize(tbnVarying * (255.0/128.0 * texture2D(normalMap, texCoordVarying).xyz - 1));
    vec3 lightDir = normalize(light.position - positionVarying);
    float diff = saturate(dot(norm, lightDir));
    vec3 diffuse = light.color * diff * texture2D(albedoMap, texCoordVarying).xyz;
    
 
    // specular
    // Phong
/*
    vec3 viewDir = normalize(viewPos - positionVarying);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(saturate(dot(viewDir, reflectDir)), max(shininess+1, 1)); // +1 because if shinines is lowe than 1 it clips
    vec3 specular = light.color * spec * texture2D(specularMap, texCoordVarying).x;
*/
    // Blinn
    vec3 viewDir = normalize(viewPos - positionVarying);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(saturate(dot(halfDir, norm)), max(shininess+1, 1));
    vec3 specular = light.color * spec * texture2D(specularMap, texCoordVarying).x;

    // attenuation
    float distance    = length(light.position - positionVarying);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse  *= attenuation;
    specular *= attenuation;   
        
    vec3 result = (diffuse + specular) * light.intensity;
    gl_FragColor = vec4(result, 1.0);
}