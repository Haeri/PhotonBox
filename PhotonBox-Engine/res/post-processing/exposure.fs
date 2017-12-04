#version 120 core

uniform sampler2D renderTexture;
uniform sampler2D exposureSample;

varying vec2 texCoordVarying;

const float minLum = 0.001;
const float maxLum = 1000;

float getLum(vec4 color){
	return 0.299*color.r + 0.587* color.g + 0.114* color.b;
}

void main(){             
	vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
	
	//float exposure = sqrt(8.0 * (getLum(texture2D(exposureSample, texCoordVarying)) + 0.25));
	float exposure = 0.18/getLum(texture2D(exposureSample, texCoordVarying));

	//gl_FragColor = vec4(hdrColor * vec3(pow(2, exposure)), 1);
	//gl_FragColor = vec4(vec3(1) - exp(-hdrColor * exposure), 1);
	gl_FragColor = vec4(hdrColor * exposure, 1);
	//gl_FragColor = vec4(vec3(1.0) - exp(-1 * hdrColor * (0.18/(min(max(exposure, minLum), maxLum)))), 1);
}