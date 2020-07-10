/*
Based on ACES curve from https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
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











