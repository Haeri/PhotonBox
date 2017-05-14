$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D fbo_texture;

varying vec4 texCoordVarying;
varying vec2 v_blurTexCoords[14];

void main()
{
    gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 0])*0.0044299121055113265;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 1])*0.00895781211794;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 2])*0.0215963866053;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 3])*0.0443683338718;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 4])*0.0776744219933;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 5])*0.115876621105;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 6])*0.147308056121;
    gl_FragColor += texture2D(fbo_texture, texCoordVarying.st )*0.159576912161;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 7])*0.147308056121;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 8])*0.115876621105;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[ 9])*0.0776744219933;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[10])*0.0443683338718;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[11])*0.0215963866053;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[12])*0.00895781211794;
    gl_FragColor += texture2D(fbo_texture, v_blurTexCoords[13])*0.0044299121055113265;
    
}
