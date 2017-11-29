#version 120

uniform sampler2D renderTexture;
uniform sampler2D luminanceSample;

uniform int maxMip;

varying vec2 texCoordVarying;

const float minLum = 0.01;
const float maxLum = 5;

void main(){             
	vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
	vec3 color = texture2DLod(luminanceSample, texCoordVarying, maxMip).rgb;
	float lum = 0.299*color.r + 0.587* color.g + 0.114* color.b;

	gl_FragColor = vec4(vec3(1.0) - exp(-1 * hdrColor * (0.18/(min(max(lum, minLum), maxLum)))), 1);
}  


