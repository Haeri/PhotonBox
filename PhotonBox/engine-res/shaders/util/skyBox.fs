#version 120

uniform samplerCube skybox;
uniform float mip = 0;
uniform float intensity;


varying vec3 texCoordVarying;

void main(){
	gl_FragColor = vec4(textureCubeLod(skybox, texCoordVarying, mip).rgb, 1) * intensity;
	gl_FragDepth = 1;
}
