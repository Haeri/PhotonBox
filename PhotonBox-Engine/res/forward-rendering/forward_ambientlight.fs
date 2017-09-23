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
uniform samplerCube skyBoxLod0;
uniform samplerCube skyBoxLod1;
uniform samplerCube skyBoxLod2;
uniform samplerCube skyBoxLod3;
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


    //float roughness = max(exp(texture2D(specularMap, texCoordVarying).x) - 1.5, 0);
	float roughness = texture2D(specularMap, texCoordVarying).x;
	float mix = 0;
    vec4 lower;
    vec4 higher;

    if(roughness > 0.666){
    	higher = textureCube(skyBoxLod0, reflectDir);
    	lower = textureCube(skyBoxLod1, reflectDir);
    	mix = (roughness - 0.666) / 0.444;
    }else if(roughness > 0.333){
       	higher = textureCube(skyBoxLod1, reflectDir);
    	lower = textureCube(skyBoxLod2, reflectDir);
    	mix = (roughness - 0.333) / 0.777;
    }else{
        higher = textureCube(skyBoxLod2, reflectDir);
    	lower = textureCube(skyBoxLod3, reflectDir);
    	mix = roughness / 0.333;
    }
		
	vec4 col2 =	vec4(light.color, 1);
    vec4 col = (textureCube(skyBoxLod3, norm) /* vec4(light.color, 1) */)  * texture2D(albedoMap, texCoordVarying) * light.intensity;
    vec4 refcol = mix(lower, higher, mix);

	gl_FragColor = mix(col, refcol, roughness) * texture2D(aoMap, texCoordVarying).x + texture2D(emissionMap, texCoordVarying);


	gl_FragColor = textureCube(skyBoxLod3, norm);
}