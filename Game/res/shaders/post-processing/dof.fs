#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform float depth;

uniform sampler2D renderTexture;
uniform sampler2D gPosition;

const float weight_3x3[9] = float[9](0.077847, 0.123317, 0.077847, 0.123317, 0.195346, 0.123317, 0.077847, 0.123317, 0.077847);
const float weight_5x5[25] = float[25](0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 0.015019, 0.059912, 0.094907, 0.059912, 0.015019, 0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 0.015019, 0.059912, 0.094907, 0.059912, 0.015019, 0.003765, 0.015019, 0.023792, 0.015019, 0.003765);
const float weight_7x7[49] = float[49](0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036, 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363, 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446, 0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291, 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446, 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363, 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036);
const float weight_9x9[81] = float[81](0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0, 0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362, 0.000036, 0.000001, 0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014, 0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055, 0.000088, 0.002289, 0.023205, 0.092566, 0.146634, 0.092566, 0.023205, 0.002289, 0.000088, 0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055, 0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014, 0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362, 0.000036, 0.000001, 0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0);

vec2 texelSize;
const float boost = 1.6;

vec4 c_3x3();
vec4 c_5x5();
vec4 c_7x7();
vec4 c_9x9();

void main() 
{


	float c_depth = -texture(gPosition, TexCoords).z;
	//float dist = abs(c_depth - depth);
	float dist = abs(c_depth - 0);
	texelSize = 1.0 / textureSize(renderTexture, 0);
    
    
	vec4 ret;

	if(dist < 2)
	{
		ret = texture(renderTexture, TexCoords).rgba;
	}
	else if(dist < 4)
	{
		ret = c_3x3();	
	}
	else if(dist < 6)
	{
		ret = c_5x5();	
	}
	else if(dist < 8)
	{
		ret = c_7x7();	
	}
	else if(dist >= 8)
	{
		ret = c_9x9();
	}	

	FragColor = ret;
}  

vec4 c_3x3()
{
	vec4 ret;

    for (int y = -1; y <= 1; ++y) 
    {
		for (int x = -1; x <= 1; ++x) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            ret += texture(renderTexture, TexCoords + (offset * boost)).rgba * weight_3x3[(y+1) * 3 + (x+1)];
            //ret += texture(renderTexture, TexCoords + offset).rgba * (1/9.0);
        }
    }

    //ret += vec4(0, 1, 0, 1);

    return ret;
 }

vec4 c_5x5()
{
	vec4 ret;

	for (int y = -2; y <= 2; ++y) 
    {
		for (int x = -2; x <= 2; ++x) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            ret += texture(renderTexture, TexCoords + (offset * boost)).rgba * weight_5x5[(y+2) * 5 + (x+2)];
            //ret += texture(renderTexture, TexCoords + offset).rgba * (1/25.0);
        }
    }
	//ret += vec4(1, 1, 0, 1);

	return ret;
}

vec4 c_7x7()
{
	vec4 ret;
	
	for (int y = -3; y <= 3; ++y) 
    {
		for (int x = -3; x <= 3; ++x) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            ret += texture(renderTexture, TexCoords + (offset * boost)).rgba * weight_7x7[(y+3) * 7 + (x+3)];
            //ret += texture(renderTexture, TexCoords + offset).rgba * (1/49.0);
        }
    }

	//ret += vec4(0, 0, 1, 1);

	return ret;
}

vec4 c_9x9()
{
	vec4 ret;
	
	for (int y = -4; y <= 4; ++y) 
    {
		for (int x = -4; x <= 4; ++x) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            ret += texture(renderTexture, TexCoords + (offset * boost)).rgba * weight_9x9[(y+4) * 9 + (x+4)];
            //ret += texture(renderTexture, TexCoords + (offset * boost)).rgba * (1/81.0);
        }
    }
    //ret += vec4(1, 0, 0, 1);

	return ret;
}