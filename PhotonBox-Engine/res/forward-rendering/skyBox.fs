#version 120

uniform samplerCube skybox;

varying vec3 texCoordVarying;

void main(){
	gl_FragColor = textureCube(skybox, texCoordVarying);
}