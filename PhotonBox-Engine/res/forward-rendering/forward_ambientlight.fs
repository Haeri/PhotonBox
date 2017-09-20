#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;
uniform sampler2D albedoMap;
uniform sampler2D emissionMap;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = vec4(light.color, 1) * texture2D(albedoMap, texCoordVarying) * light.intensity + texture2D(emissionMap, texCoordVarying);
}