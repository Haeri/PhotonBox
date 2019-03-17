#version 330
out vec4 FragColor;

in vec2 TexCoords;

//uniform float depth;

uniform sampler2D rendertexture;
uniform sampler2D gPosition;

const float weight_3x3[9] = float[9](0.077847, 0.123317, 0.077847, 0.123317, 0.195346, 0.123317, 0.077847, 0.123317, 0.077847);
const float weight_5x5[25] = float[25](0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 0.015019, 0.059912, 0.094907, 0.059912, 0.015019, 0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 0.015019, 0.059912, 0.094907, 0.059912, 0.015019, 0.003765, 0.015019, 0.023792, 0.015019, 0.003765);
const float weight_7x7[49] = float[49](0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036, 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363, 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446, 0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291, 0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446, 0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363, 0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036);
const float weight_9x9[81] = float[81](0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0, 0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362, 0.000036, 0.000001, 0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014, 0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055, 0.000088, 0.002289, 0.023205, 0.092566, 0.146634, 0.092566, 0.023205, 0.002289, 0.000088, 0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055, 0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014, 0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362, 0.000036, 0.000001, 0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0);

const vec2 rand_vecs[100] = vec2[100](vec2(-0.05, 0.23), vec2(0.28, 0.25), vec2(-0.88, 0.08), vec2(0.11, 0.14), vec2(-0.69, 0.98), vec2(-0.25, 0.72), vec2(-0.10, 0.36), vec2(-0.57, -0.10), vec2(0.51, -0.48), vec2(0.08, -0.71), vec2(0.02, 0.81), vec2(0.23, 0.05), vec2(0.02, -0.04), vec2(0.58, -0.38), vec2(-0.46, -0.30), vec2(0.08, -0.62), vec2(-0.79, -0.84), vec2(0.47, -0.61), vec2(0.18, -0.80), vec2(-0.81, -0.01), vec2(-0.22, 0.16), vec2(-0.08, -0.60), vec2(-0.38, -0.99), vec2(-0.01, 0.35), vec2(-0.19, -0.80), vec2(-0.12, -0.68), vec2(-0.79, -0.79), vec2(0.31, -0.45), vec2(-0.80, 0.02), vec2(0.23, 0.09), vec2(-0.57, -0.12), vec2(0.58, -0.88), vec2(0.18, 0.62), vec2(0.01, -0.40), vec2(-0.22, -0.52), vec2(0.69, 0.74), vec2(-0.93, 0.12), vec2(-0.61, -0.75), vec2(-0.33, 0.57), vec2(-0.31, 0.85), vec2(-0.59, -0.15), vec2(-0.51, -0.47), vec2(0.90, -0.67), vec2(-0.22, 0.93), vec2(-0.31, -0.18), vec2(-0.88, 0.34), vec2(0.64, 0.31), vec2(-0.26, 0.89), vec2(-0.30, 0.45), vec2(-0.74, -0.52), vec2(-0.47, -0.91), vec2(-0.86, -0.99), vec2(-0.22, -0.35), vec2(-0.10, -0.25), vec2(-0.16, -0.47), vec2(0.92, -0.86), vec2(-0.85, -0.53), vec2(0.37, 0.39), vec2(0.87, 0.24), vec2(0.93, -0.75), vec2(0.07, -0.08), vec2(-0.82, 0.67), vec2(-0.82, 0.67), vec2(-0.80, -0.77), vec2(0.46, -0.49), vec2(-0.41, 0.73), vec2(-0.33, -0.57), vec2(-0.65, 0.97), vec2(0.37, -0.88), vec2(0.35, 0.36), vec2(0.00, 0.65), vec2(-0.21, 0.98), vec2(0.76, 0.49), vec2(0.86, 0.53), vec2(0.64, 0.33), vec2(0.26, -0.27), vec2(-0.91, -0.47), vec2(-0.50, 0.24), vec2(-0.64, -0.70), vec2(-0.41, 0.75), vec2(-0.19, 0.46), vec2(0.14, 0.85), vec2(-0.16, -0.03), vec2(0.19, -0.84), vec2(-0.44, 0.64), vec2(-0.71, -0.07), vec2(0.98, -0.42), vec2(0.03, -1.00), vec2(0.91, 0.67), vec2(0.53, -0.10), vec2(0.07, 0.95), vec2(-0.68, -0.77), vec2(-0.47, 0.50), vec2(-0.16, -0.36), vec2(0.29, -0.47), vec2(-0.63, 0.94), vec2(-0.51, -0.76), vec2(-0.52, -0.69), vec2(-0.19, 0.94), vec2(0.84, 0.87));

vec2 texelSize;
const float boost = 2;
const float shallow = 1;
const bool debug = false;
const float d = 0;

float c_depth;
vec4 c_color;


vec4 c_3x3(float lod);
vec4 c_5x5();
vec4 c_7x7();
vec4 c_9x9();

void main() 
{


	c_depth = -textureLod(gPosition, TexCoords, 0).z;
	c_color = textureLod(rendertexture, TexCoords, 0).rgba;
	//float dist = abs(c_depth - depth);
	float dist = abs(c_depth - d);
	texelSize = 1.0 / textureSize(rendertexture, 0);
    
    
	vec4 ret;

	if(dist < shallow && c_depth != 0)
	{
		ret = textureLod(rendertexture, TexCoords, 0).rgba;
	}
	else if(dist < shallow * 2  && c_depth != 0)
	{
		ret = c_3x3(1);	
	}
	else if(dist < shallow * 3  && c_depth != 0)
	{
        //ret = c_3x3(2); 
		ret = c_5x5();	
	}
	else if(dist < shallow * 4  && c_depth != 0)
	{
        //ret = c_3x3(3); 
		ret = c_7x7();	
	}
	else if(dist >= shallow * 4 || c_depth == 0)
	{
        
       // ret = c_3x3(4); 

		ret = c_9x9();
	}	


	FragColor = ret;
    //FragColor = textureLodLod(rendertexture, TexCoords, 2);
	
	/*
	if(-textureLod(gPosition, TexCoords).z == 0)
		FragColor = vec4(1, 0, 0, 1);
	else
		FragColor = vec4(0, 0, 0, 1);
	*/
/*
	if(textureLodSize(rendertexture, 0) == textureLodSize(gPosition, 0))
	{
		FragColor = vec4(0, 1, 0, 1);
	}
	else
	{
		FragColor = vec4(1, 0, 0, 1);
	}
	*/
}  


vec4 c_3x3(float lod)
{
	vec4 ret;

    for (int y = -1; y <= 1; ++y) 
    {
		for (int x = -1; x <= 1; ++x) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;

            float i_depth = textureLod(gPosition, TexCoords + (offset * boost + rand_vecs[(y+1) * 3 + (x+1)] * 0.001), lod).z;

            if(i_depth == 0 || -i_depth > shallow)
            {
            	ret += textureLod(rendertexture, TexCoords + (offset * boost + rand_vecs[(y+1) * 3 + (x+1)] * 0.001), lod).rgba * weight_3x3[(y+1) * 3 + (x+1)];
            }
            else
            {
            	ret += c_color * weight_3x3[(y+1) * 3 + (x+1)];
            }
        }
    }

    if(debug)
    	ret += vec4(0, 1, 0, 1);

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

            float i_depth = textureLod(gPosition, TexCoords + (offset * boost), 1).z;

            if(i_depth == 0 || -i_depth > shallow)
            {
            	ret += textureLod(rendertexture, TexCoords + (offset * boost), 1).rgba * weight_5x5[(y+2) * 5 + (x+2)];
            }
            else
            {
            	ret += c_color * weight_5x5[(y+2) * 5 + (x+2)];
            }
        }
    }

    if(debug)
		ret += vec4(1, 1, 0, 1);

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

            float i_depth = textureLod(gPosition, TexCoords + (offset * boost), 2).z;

            if(i_depth == 0 || -i_depth > shallow)
            {
            	ret += textureLod(rendertexture, TexCoords + (offset * boost), 2).rgba * weight_7x7[(y+3) * 7 + (x+3)];
            }
            else
            {
            	ret += c_color * weight_7x7[(y+3) * 7 + (x+3)];
            }
        }
    }

    if(debug)
		ret += vec4(0, 0, 1, 1);

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

            float i_depth = textureLod(gPosition, TexCoords + (offset * boost), 2).z;

            if(i_depth == 0 || -i_depth > shallow)
            {
            	ret += textureLod(rendertexture, TexCoords + (offset * boost), 2).rgba * weight_9x9[(y+4) * 9 + (x+4)];
            }
            else
            {
            	ret += c_color * weight_9x9[(y+4) * 9 + (x+4)];
            }
        }
    }

    if(debug)
    	ret += vec4(1, 0, 0, 1);

	return ret;
}