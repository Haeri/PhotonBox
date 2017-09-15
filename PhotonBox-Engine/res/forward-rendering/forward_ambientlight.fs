#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;
uniform sampler2D albedo;

varying vec2 texCoordVarying;

void main(){
	vec3 res = light.color * texture2D(albedo, texCoordVarying) * light.intensity;
	gl_FragColor = vec4(res, 1);
}