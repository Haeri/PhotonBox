#version 120

uniform sampler2D diffuse;

uniform vec4 lightPosition;
uniform vec4 lightColor;
unifrom float lightIntensity;

varying vec2 texCoordVarying;
varying vec3 normalVarying;

void main(){
	vec4 totalColor = texture2D(diffuse, texCoordVarying)
	totalColor += (lightIntensity * lightColor * dot(normalize(lightPosition - gl_Position), normalVarying));

	gl_FragColor = totalColor;
}