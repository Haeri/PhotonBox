#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 tangent;

out vec3 FragPos;
out vec3 Pos;
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;


void main()
{
    /*
    * Position and Normal are in view space
    */
 	vec4 viewPos = view * model * vec4(aPos, 1.0);
    Pos = (model * vec4(aPos, 1.0)).xyz;
    FragPos = viewPos.xyz; 
    TexCoords = aTexCoords;
    
    vec3 n = normalize((model * vec4(aNormal, 0.0)).xyz);
    vec3 t = normalize((model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 bitangent = cross(n, t);
    TBN = mat3(t, bitangent, n);

    gl_Position = mvp * vec4(aPos, 1.0);
}