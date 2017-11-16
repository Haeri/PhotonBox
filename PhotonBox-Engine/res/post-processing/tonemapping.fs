#version 120

uniform sampler2D renderTexture;
uniform float exposure = 0.4;

varying vec2 texCoordVarying;
    
const float gamma = 2.2;

void main(){             
    vec3 hdrColor = texture2D(renderTexture, texCoordVarying).rgb;
    // tone mapping
    vec3 result = vec3(1.0) - exp(-1 * hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    gl_FragColor = vec4(result, 1.0);
}  