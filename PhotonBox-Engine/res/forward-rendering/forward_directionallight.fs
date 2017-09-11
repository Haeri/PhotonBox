#version 120

struct DirectionalLight{
	vec3 direction;
	vec3 color;
    float intensity;
};

float shininess = 12; 

uniform DirectionalLight light;
uniform vec3 viewPos;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;

void main(){
  	// diffuse 
    vec3 norm = normalize(normalVarying);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff;  

    // specular
    vec3 viewDir = normalize(viewPos - positionVarying);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.color * spec;  

    vec3 result = (diffuse + specular) * light.intensity;
    gl_FragColor = vec4(result, 1.0);
}