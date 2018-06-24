#version 120

varying vec3 colorVarying;

void main(){
	gl_FragColor = vec4(colorVarying, 0.5);
 	//gl_FragColor = vec4(1, 0, 0, 1.0); 	
}

