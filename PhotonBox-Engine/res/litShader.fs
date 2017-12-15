#version 120

uniform sampler2D texture;
uniform vec3 color = vec3(-1, 0, 0);

varying vec2 texCoordVarying;

void main(){
	if(color.x != -1)
		gl_FragColor = vec4(color, 1);
	else
		gl_FragColor = vec4(pow(texture2D(texture, texCoordVarying).rgb, vec3(2.2)), 1);
}
