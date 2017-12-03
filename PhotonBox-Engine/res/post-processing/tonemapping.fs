#version 120 core

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