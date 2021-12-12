#version 120

uniform sampler2D renderTexture;
uniform float offset;

varying vec2 texCoordVarying;

void main(){
	vec4 color = vec4(0, 0, 0, 1); 
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y - offset * 5)) * 0.000003;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y - offset * 4)) * 0.000229;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y - offset * 3)) * 0.005977;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y - offset * 2)) * 0.060598;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y - offset * 1)) * 0.241730;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y 			)) * 0.382925;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y + offset * 1)) * 0.241730;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y + offset * 2)) * 0.060598;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y + offset * 3)) * 0.005977;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y + offset * 4)) * 0.000229;
	color += texture2D(renderTexture, vec2(texCoordVarying.x, texCoordVarying.y + offset * 5)) * 0.000003;
	gl_FragColor = color;
}
