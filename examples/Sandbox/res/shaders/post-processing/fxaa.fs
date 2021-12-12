#version 120

varying vec2 texCoordVarying;

uniform sampler2D renderTexture;
uniform float screenWidth;
uniform float screenHeight;

//uniform vec3 R_inverseFilterTextureSize;
uniform float R_fxaaSpanMax;
uniform float R_fxaaReduceMin;
uniform float R_fxaaReduceMul;


void main()
{
	vec2 texelSize = 1.0 / vec2(screenWidth, screenHeight);
	
	vec3 luma = vec3(0.299, 0.587, 0.114);	
	float lumaTL = dot(luma, texture2D(renderTexture, texCoordVarying.xy + (vec2(-1.0, -1.0) * texelSize)).xyz);
	float lumaTR = dot(luma, texture2D(renderTexture, texCoordVarying.xy + (vec2(1.0, -1.0) * texelSize)).xyz);
	float lumaBL = dot(luma, texture2D(renderTexture, texCoordVarying.xy + (vec2(-1.0, 1.0) * texelSize)).xyz);
	float lumaBR = dot(luma, texture2D(renderTexture, texCoordVarying.xy + (vec2(1.0, 1.0) * texelSize)).xyz);
	float lumaM  = dot(luma, texture2D(renderTexture, texCoordVarying.xy).xyz);

	
	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
	
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(R_fxaaSpanMax, R_fxaaSpanMax), 
		max(vec2(-R_fxaaSpanMax, -R_fxaaSpanMax), dir * inverseDirAdjustment));
	
	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));
	
	//float dirStep = max(step(1.0, abs(dir.x)), step(1.0, abs(dir.y)));
	//dir.x = dir.x * dirStep;
	//dir.y = dir.y * dirStep;
	
	dir = dir * texelSize;

	vec3 result1 = (1.0/2.0) * (
		texture2D(renderTexture, texCoordVarying.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture2D(renderTexture, texCoordVarying.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture2D(renderTexture, texCoordVarying.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture2D(renderTexture, texCoordVarying.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	
	vec4 color;
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		color = vec4(result1, 1.0);
	else
		color = vec4(result2, 1.0);

	gl_FragColor = color;

}