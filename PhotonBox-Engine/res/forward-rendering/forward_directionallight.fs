#version 120

struct DirectionalLight{
	vec3 direction;
	vec3 color;
    float intensity;
};


uniform DirectionalLight light;
uniform float shininess; 
uniform sampler2D albedo;
uniform vec3 viewPos;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;

float saturate(float value){
    return max(min(value, 1), 0);
}

void main(){
  	// diffuse 
    vec3 norm = normalize(normalVarying);
    vec3 lightDir = normalize(light.direction);
    float diff = saturate(dot(norm, lightDir));
    vec3 diffuse = light.color * diff;  

    // specular
    vec3 viewDir = normalize(viewPos - positionVarying);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(saturate(dot(viewDir, reflectDir)), max(shininess, 1));
    vec3 specular = light.color * spec;

    vec3 result = (diffuse /* texture2D(albedo, texCoordVarying) */ + specular) * light.intensity;
    gl_FragColor = vec4(result, 1.0);
}