#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D fbo_original;
uniform sampler2D fbo_blurTexture;

uniform float intensity;

varying vec4 texCoordVarying;

void main()
{

	vec4 col1 = texture2D(fbo_original, texCoordVarying.st);
	vec4 col2 = texture2D(fbo_blurTexture, texCoordVarying.st);

	gl_FragColor = col1 + col2 * intensity;
}