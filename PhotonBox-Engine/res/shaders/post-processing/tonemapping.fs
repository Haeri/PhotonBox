/*
#version 120

uniform sampler2D renderTexture;

varying vec2 texCoordVarying;
    
const float gamma = 2.2;

void main(){             
    vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
    // tone mapping
    //vec3 result = vec3(1.0) - exp(-1 * hdrColor * exposure);
    // also gamma correct while we're at it       
    vec3 result = pow(hdrColor, vec3(1.0 / gamma));
    gl_FragColor = vec4(result, 1.0);
}  


*/
#version 120

uniform sampler2D renderTexture;

varying vec2 texCoordVarying;
const float gamma = 2.2;

float a = 2.51f;
float b = 0.03f;
float c = 2.43f;
float d = 0.59f;
float e = 0.14f;

vec3 aces(vec3 inColor){
    return clamp((inColor*(a*inColor+b))/(inColor*(c*inColor+d)+e), vec3(0), vec3(1));
}

void main(){
	vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
    vec3 toneMapped = aces(hdrColor);
    vec3 gammeCorrected = pow(toneMapped, vec3(1.0 / gamma));
    
    gl_FragColor = vec4(gammeCorrected, 1.0);
}











