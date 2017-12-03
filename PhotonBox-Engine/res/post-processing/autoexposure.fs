#version 120 core

uniform sampler2D luminanceSampleCurrent;
uniform sampler2D luminanceSampleLast;

uniform float delteTime;
uniform float adaptationSpeed;

uniform int maxMip;

varying vec2 texCoordVarying;

float getLum(vec4 color){
	return 0.299*color.r + 0.587* color.g + 0.114* color.b;
}

void main(){             
	float lumC = getLum(texture2DLod(luminanceSampleCurrent, texCoordVarying, maxMip));
	float lumL = getLum(texture2D(luminanceSampleLast, texCoordVarying));
	float lum = mix(lumL, lumC, adaptationSpeed * delteTime * 5);
	float clampLum = min(max(lum, 0.001), 1000);

	gl_FragColor = vec4(vec3(clampLum), 1);
	//vec4(vec3(lumL + (lumC - lumL) * ( 1.0 - pow(0.98, adaptationSpeed * delteTime))), 1);

	//gl_FragColor = vec4(vec3(lumL + (lumC - lumL) * ( 1.0 - pow(0.98, adaptationSpeed * delteTime))), 1);
}