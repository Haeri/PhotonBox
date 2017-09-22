#version 120

struct AmbientLight{
	vec3 color;
    float intensity;
};

uniform AmbientLight light;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;
uniform samplerCube skyBoxSpec;
uniform samplerCube skyBoxDif;
uniform mat4 modelMatrix;
uniform vec3 viewPos;

varying vec2 texCoordVarying;
varying vec3 positionVarying;
varying vec3 normalVarying;
varying mat3 tbnVarying;

void main(){
    vec3 norm = normalize(tbnVarying * (255.0/128.0 * texture2D(normalMap, texCoordVarying).xyz - 1));
    vec3 viewDir = normalize(positionVarying - viewPos);
    vec3 reflectDir = reflect(viewDir, norm); 

    vec4 col = (textureCube(skyBoxDif, norm) /* vec4(light.color, 1) */)  * texture2D(albedoMap, texCoordVarying) * light.intensity;
    vec4 refcol = textureCube(skyBoxSpec, reflectDir);
    float roughness = max(exp(texture2D(specularMap, texCoordVarying).x) - 1.5, 0);
	gl_FragColor = mix(col, refcol, roughness) * texture2D(aoMap, texCoordVarying).x + texture2D(emissionMap, texCoordVarying);
}