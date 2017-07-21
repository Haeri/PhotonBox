#version 120

varying vec3 color_varying;

void main(){
	gl_FragColor = vec4(color_varying, 1);
}