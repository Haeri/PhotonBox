#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;
uniform sampler2D diffuse;

varying vec2 texCoordVarying;

void main(){
	gl_FragColor = vec4(light.intensity * light.color.xyz, 1) * texture2D(diffuse, texCoordVarying);
}