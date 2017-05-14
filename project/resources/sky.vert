$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;

varying mediump vec4 pos;
varying vec4 texCoordVarying;


void main()
{
    pos = ModelMatrix * Position;  // vertex position in eye coordinates
    
    texCoordVarying = pos;
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}