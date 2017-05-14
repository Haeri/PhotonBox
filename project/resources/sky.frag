$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform samplerCube CubeMap;

varying lowp vec4 texCoordVarying;

void main()
{
    lowp vec4 color = textureCube(CubeMap, texCoordVarying.xyz);
    gl_FragColor = color;
}