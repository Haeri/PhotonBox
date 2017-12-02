#version 120

uniform sampler2D renderTexture;
uniform sampler2D luminanceSampleCurrent;
uniform sampler2D luminanceSampleLast;

uniform float delteTime;
uniform float adaptationSpeed;

uniform int maxMip;

varying vec2 texCoordVarying;

const float minLum = 0.001;
const float maxLum = 1000;

void main(){             
	vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
	
	vec3 colorC = texture2DLod(luminanceSampleCurrent, texCoordVarying, maxMip).rgb;
	float lumC = 0.299*colorC.r + 0.587* colorC.g + 0.114* colorC.b;
	vec3 colorL = texture2DLod(luminanceSampleLast, texCoordVarying, maxMip).rgb;
	float lumL = 0.299*colorL.r + 0.587* colorL.g + 0.114* colorL.b;
	float lum = mix(lumL, lumC, delteTime * adaptationSpeed);

	gl_FragColor = vec4(vec3(1.0) - exp(-1 * hdrColor * (0.18/(min(max(lum, minLum), maxLum)))), 1);
}