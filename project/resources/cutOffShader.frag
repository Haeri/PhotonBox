$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D fbo_texture;
uniform float thresHold;

varying vec4 texCoordVarying;

void main()
{

	vec4 col = texture2D(fbo_texture, texCoordVarying.st);
	float brightness = (0.2126 *col.x + 0.7152 * col.y + 0.0722 * col.z);
    if(brightness > thresHold)
    	gl_FragColor = col;
    else
		gl_FragColor = vec4(0, 0, 0, 1);    	
}