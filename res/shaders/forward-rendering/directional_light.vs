/*
Based on the pbr shader from https://github.com/JoeyDeVries/LearnOpenGL
*/

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

out vec2 texCoordVarying;
out vec3 positionVarying;
out mat3 tbnVarying;
out vec4 fragPosLightSpace;

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
