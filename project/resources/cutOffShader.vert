#ifdef GL_ES
precision lowp float;
#endif

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec4 TexCoord;

varying vec4 texCoordVarying;

void main()
{
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ModelViewMatrix * Position;
}