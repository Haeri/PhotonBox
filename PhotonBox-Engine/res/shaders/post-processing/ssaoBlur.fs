#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D original;
uniform sampler2D ssaoInput;


uniform float screenWidth;
uniform float screenHeight;

void main() 
{
    vec3 orig = texture(original, TexCoords).rgb;
    vec2 texelSize = 1.0 / vec2(screenWidth, screenHeight);
    float result = 0.0;
    for (int x = -1; x <= 2; ++x) 
    {
        for (int y = -1; y <= 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, TexCoords + offset).r;
        }
    }
     result = result / (4.0 * 4.0);

     FragColor = vec4(orig * result, 1);
}  