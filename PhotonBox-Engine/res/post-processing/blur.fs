#version 120

uniform sampler2D renderTexture;
uniform float aspectRatio;
uniform float offset;

varying vec2 texCoordVarying;

void main(){
	float offsetX = offset;
	float offsetY = offset * aspectRatio;

	vec4 colors[9]; 
	colors[0] = texture2D(renderTexture, vec2(texCoordVarying.x - offsetX, texCoordVarying.y + offsetY)) * 1;
	colors[1] = texture2D(renderTexture, vec2(texCoordVarying.x			, texCoordVarying.y + offsetY)) * 2;
	colors[2] = texture2D(renderTexture, vec2(texCoordVarying.x + offsetX, texCoordVarying.y + offsetY)) * 1;
	colors[3] = texture2D(renderTexture, vec2(texCoordVarying.x - offsetX, texCoordVarying.y ))		   * 2;
	colors[4] = texture2D(renderTexture, vec2(texCoordVarying.x 		, texCoordVarying.y ))		   * 4;
	colors[5] = texture2D(renderTexture, vec2(texCoordVarying.x + offsetX, texCoordVarying.y ))		   * 2;
	colors[6] = texture2D(renderTexture, vec2(texCoordVarying.x - offsetX, texCoordVarying.y - offsetY)) * 1;
	colors[7] = texture2D(renderTexture, vec2(texCoordVarying.x 		, texCoordVarying.y - offsetY)) * 2;
	colors[8] = texture2D(renderTexture, vec2(texCoordVarying.x + offsetX, texCoordVarying.y - offsetY)) * 1;

	vec4 sum = colors[0] + colors[1] + colors[2] + colors[3] + colors[4] + colors[5] + colors[6] + colors[7] + colors[8]; 
	gl_FragColor = (sum/16);
}
