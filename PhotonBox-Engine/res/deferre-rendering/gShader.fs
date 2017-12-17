#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
}