#version 120

uniform sampler2D diffuse;

uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform float lightIntensity;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;

void main(){
	vec4 totalColor = texture2D(diffuse, texCoordVarying);
	totalColor += (lightIntensity * lightColor * dot(normalize(lightPosition - positionVarying), normalVarying));

	//gl_FragColor = totalColor;
	gl_FragColor = vec4(1, 0, 0, 1);
}