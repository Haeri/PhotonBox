#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;
uniform sampler2D albedo;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = vec4(light.color, 1) * texture2D(albedo, texCoordVarying) * light.intensity;
}