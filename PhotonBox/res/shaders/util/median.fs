#version 330

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;

void sort(inout vec3 arr[5], int a, int b) { 
    vec3 tmp = min(arr[a], arr[b]); 
    arr[b] = arr[a] + arr[b] - tmp;
    arr[a] = tmp; 
}

void main() 
{
	vec4 c_color = textureLod(renderTexture, TexCoords, 0).rgba;
	vec2 texelSize = 1.0 / textureSize(renderTexture, 0);
    
    vec3 arr[5];
    arr[0] = texture(renderTexture, vec2(TexCoords.x-1 * texelSize.x,   TexCoords.y)                ).rgb;
    arr[1] = texture(renderTexture, vec2(TexCoords.x,                   TexCoords.y+1 * texelSize.y)).rgb;
    arr[2] = texture(renderTexture, vec2(TexCoords.x+1 * texelSize.x,   TexCoords.y)                ).rgb;
    arr[3] = texture(renderTexture, vec2(TexCoords.x,                   TexCoords.y-1 * texelSize.y)).rgb;
    arr[4] = texture(renderTexture, vec2(TexCoords.x,                   TexCoords.y)                ).rgb;

    for (int n = 4; n > 0; --n) {
        for (int i = 0; i < n; ++i) {
            sort(arr, i, n);
        }
    }

	FragColor = vec4(arr[2], c_color.a);
}