/*
Based on the pbr shader from https://github.com/JoeyDeVries/LearnOpenGL
*/

#version 130

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec3 tangent;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying mat3 tbnVarying;
varying vec4 fragPosLightSpace;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
	positionVarying = (modelMatrix * vec4(position, 1.0)).xyz;
	texCoordVarying = uv;
	//fragPosLightSpace = lightSpaceMatrix * modelMatrix * vec4(position, 1.0);
	fragPosLightSpace = lightSpaceMatrix * vec4(vec3(modelMatrix * vec4(position, 1.0)), 1.0);

	vec3 n = normalize((modelMatrix * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((modelMatrix * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 bitangent = cross(n, t);
    tbnVarying = mat3(t, bitangent, n);
}
