#version 120 core

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;

uniform sampler2D albedoMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;

varying vec2 texCoordVarying;

void main(){
    vec3 albedo     = pow(texture2D(albedoMap, texCoordVarying).rgb, vec3(2.2));
    vec3 emission   = texture2D(emissionMap, texCoordVarying).rgb;
    float ao        = texture2D(aoMap, texCoordVarying).r;

	gl_FragColor = vec4(light.color * light.intensity * albedo * ao + emission, 1);
}