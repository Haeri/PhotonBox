#version 120

uniform sampler2D renderTexture;
uniform sampler2D exposureSample;

varying vec2 texCoordVarying;

float getLum(vec4 color){
	return 0.299*color.r + 0.587* color.g + 0.114* color.b;
}

void main(){             
	vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
	float exposure = 0.18/getLum(texture2D(exposureSample, texCoordVarying));

	gl_FragColor = vec4(hdrColor * exposure, 1);
}
