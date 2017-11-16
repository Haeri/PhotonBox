#version 120

uniform samplerCube skybox;
uniform float mip = 0;


varying vec3 texCoordVarying;

void main(){
	gl_FragColor = vec4(pow(textureCubeLod(skybox, texCoordVarying, mip).rgb, vec3(2.2)), 1);
}