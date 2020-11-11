#version 420

/*
MIT License

Copyright (c) [2019] [Feodor Volguine]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define PI 3.14159265359f
#define EPSILON 0.000001f
#define LUMA_COEFFICIENTS vec3(0.2126f, 0.7152f, 0.0722f)
//Feel free to play around with these values. My jitter sequence has 16 elements so I use 1 - (1 / 16) = 0.9375 for the max weight
#define WEIGHT_MINIMUM 0.8f
#define WEIGHT_MAXIMUM 0.9375f

layout(location = 0) out vec4 FragColor;

in vec2 texCoordsFrag;

//Input texture from current frame (it's called low-res because I am using this algorithm for supersampling)
uniform sampler2D lowResTexture;
//Result from previous frame
uniform sampler2D previousLowResTexture;
//Non-linear depth
uniform sampler2D gPosition;
//Encoded normals and velocity
uniform sampler2D normalVelocity;

//float LinearizeDepth(float inputDepth) { return (2.0f * nearPlane) / (farPlane + nearPlane - inputDepth * (farPlane - nearPlane)); }
float ComputeLuminance(vec3 rgb) { return dot(LUMA_COEFFICIENTS, rgb); }
//https://software.intel.com/en-us/node/503873
vec3  RGBtoYCOCG(vec3 c)
{ return vec3(c.x / 4.0f + c.y / 2.0f + c.z / 4.0f, c.x / 2.0f - c.z / 2.0f, -c.x / 4.0f + c.y / 2.0f - c.z / 4.0f); }
vec3  YCOCGtoRGB(vec3 c)
{ return clamp(vec3(c.x + c.y - c.z, c.x + c.z, c.x - c.y - c.z), 0.0f, 1.0f); }
//https://gpuopen.com/optimized-reversible-tonemapper-for-resolve/
vec3  Tonemap(vec3 c) { return c / (max(c.r, max(c.g, c.b)) + 1.0f); }
vec3  InverseTonemap(vec3 c) { return c / (1.0f - max(c.r, max(c.g, c.b))); }
float BlackmanHarris(float x);
vec2  FetchClosestInverseVelocity(vec2 texCoords);
vec4  Reproject(sampler2D previousInputTexture, vec2 previousTexCoords);
void  VarianceAABB(inout vec3 aabbMin, inout vec3 aabbMax, vec3 point, vec3 mu, vec3 mu2);
void  TightenAABBchroma(inout vec3 mu, inout vec3 mu2, inout vec3 aabbMin, inout vec3 aabbMax, vec3 currentPixelValue);
vec4  ClipAABB(vec3 neighborhoodMin, vec3 neighborhoodMax, vec4 currentValue, vec4 previousValue);
vec4  FetchSampleValue(sampler2D inputTexture, vec2 texCoords);
float UnbiasedLuminanceWeight(float currentLuminance, float previousLuminance, vec3 aabbMax);
vec4  TemporalResolve(sampler2D inputTexture, sampler2D previousInputTexture);

vec2 resolutionInput  = textureSize(lowResTexture,  0);
vec2 resolutionOutput = textureSize(normalVelocity, 0);
//Replace these with uniforms because division is slow
vec2 texelSizeInput  = 1.0f / resolutionInput;
vec2 texelSizeOutput = 1.0f / resolutionOutput;

void main()
{
	vec4 result = TemporalResolve(lowResTexture, previousLowResTexture);
	//result.a = texture(lowResTexture, texCoordsFrag).a;
	
	FragColor = result;
}

float BlackmanHarris(float x)
{
    x = 1.0f - x;

    float a0 = 0.35875f;
    float a1 = 0.48829f;
    float a2 = 0.14128f;
    float a3 = 0.01168f;
    return clamp(a0 - a1 * cos(PI * x) + a2 * cos(2 * PI * x) - a3 * cos(3 * PI * x), 0.0f, 1.0f);
}

vec4 FetchSampleValue(sampler2D inputTexture, vec2 texCoords)
{
	vec4 value = texture(inputTexture, texCoords);
	value.xyz  = Tonemap(value.xyz);
	value.xyz  = RGBtoYCOCG(value.xyz);
	return value;
}

float UnbiasedLuminanceWeight(float currentLuminance, float previousLuminance, vec3 aabbMax)
{
	float unbiasedLumaDifference = abs(currentLuminance - previousLuminance) / (max(currentLuminance, max(previousLuminance, ComputeLuminance(aabbMax))) + EPSILON);
	
	float weight = 1.0f - unbiasedLumaDifference;
	weight       = weight * weight;
	weight       = mix(WEIGHT_MINIMUM, WEIGHT_MAXIMUM, weight);
	return weight;
}

//https://developer.download.nvidia.com/gameworks/events/GDC2016/msalvi_temporal_supersampling.pdf
void VarianceAABB(inout vec3 aabbMin, inout vec3 aabbMax, vec3 point, vec3 mu, vec3 mu2)
{
	//Standard deviation sigma
	vec3 sigma = sqrt(abs(mu2 - mu * mu));
	//Larger gamma is more stable but has more ghosting (values between 0.75 and 1.25 work well)
	const float gamma = 1.0f;

	//Create AABB centered around mu
	vec3 newAABBmin = mu - gamma * sigma;
	vec3 newAABBmax = mu + gamma * sigma;
	//Clamp against old AABB
	aabbMin = max(newAABBmin, aabbMin);
	aabbMax = min(newAABBmax, aabbMax);
}

//https://github.com/playdeadgames/temporal/blob/master/Assets/Shaders/TemporalReprojection.shader
void TightenAABBchroma(inout vec3 mu, inout vec3 mu2, inout vec3 aabbMin, inout vec3 aabbMax, vec3 currentPixelValue)
{
	vec2 chromaExtent  = vec2(0.25f * 0.5f * (aabbMax.x - aabbMin.x));
	vec2 chromaCenter  = currentPixelValue.yz;

	mu.yz      = chromaCenter;
	//Not sure how to shrink mu2 (squaring center chroma doesnt seem to be correct)
	//mu2.yz     = chromaCenter * chromaCenter;
	aabbMin.yz = chromaCenter - chromaExtent;
	aabbMax.yz = chromaCenter + chromaExtent;
}

//https://github.com/playdeadgames/temporal/blob/master/Assets/Shaders/TemporalReprojection.shader
vec3 ClipAABB(vec3 aabbMin, vec3 aabbMax, vec3 point)
{
	//Clips towards AABB center for better perfomance
	vec3 center   = 0.5f * (aabbMax + aabbMin);
	vec3 halfSize = 0.5f * (aabbMax - aabbMin) + EPSILON;
	//Relative position from the center
	vec3 clip     = point - center;
	//Normalize relative position
	vec3 unit     = clip / halfSize;
	vec3 absUnit  = abs(unit);
	float maxUnit = max(absUnit.x, max(absUnit.y, absUnit.z));
	if(maxUnit > 1.0f)
		return center + clip / maxUnit;
	else
		return point;
}

//Obtains closest inverse velocity in 3x3 neighborhood
//NOTE: Using longest velocity instead of closest could be better
vec2 FetchClosestInverseVelocity(vec2 texCoords, vec2 texelSize)
{
	float closestDepth     = 1.0f;
	vec2  closestTexCoords = vec2(0.0f);

	//Search 3x3 neighborhood
	for(int i = -1; i <= 1; ++i)
	{
		for(int j = -1; j <= 1; ++j)
		{
			vec2  currentTexCoords = texCoords + (vec2(i, j) * texelSize);
			//float currentDepth     = LinearizeDepth(texture(depthTexture, currentTexCoords).x);
			float currentDepth     = texture(gPosition, currentTexCoords).z;
			
			if(currentDepth < closestDepth)
			{
				closestDepth     = currentDepth;
				closestTexCoords = currentTexCoords;
			}
		}
	}

	return -texture(normalVelocity, closestTexCoords).zw;
}

vec4 Reproject(sampler2D previousInputTexture, vec2 previousTexCoords)
{
	vec4 previousPixelValue = FetchSampleValue(previousInputTexture, previousTexCoords);
	if(true)
		return previousPixelValue;
	//This doesn't work yet.
	else
	{
		vec4  valueTotal  = vec4(0.0f);
		float weightTotal = 0.0f;

		for(int i = -1; i <= 1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				vec2  sampleOffset   = vec2(j, i);
				vec2  sampleCoords   = previousTexCoords + (sampleOffset * texelSizeOutput);
				vec4  sampleValue    = FetchSampleValue(previousInputTexture, sampleCoords);
				vec2  sampleDistance = abs(sampleOffset);
				float weightSample   = BlackmanHarris(sampleDistance.x) * BlackmanHarris(sampleDistance.y);

				valueTotal  += sampleValue * weightSample;
				weightTotal += weightSample;
			}
		}

		return max(valueTotal / weightTotal, 0.0f);
	}
}

vec4 TemporalResolve(sampler2D inputTexture, sampler2D previousInputTexture)
{
	//Value of current pixel
	vec4 currentPixelValue = FetchSampleValue(lowResTexture, texCoordsFrag);
	//Screen space velocity
	vec2 velocity = FetchClosestInverseVelocity(texCoordsFrag, texelSizeOutput);

	//Texture coordinate reprojection
	vec2 previousTexCoords  = texCoordsFrag + velocity;
	vec4 previousPixelValue = Reproject(previousInputTexture, previousTexCoords);

	vec2 texelSizeInputX = vec2(texelSizeInput.x, 0.0f);
	vec2 texelSizeInputY = vec2(0.0f, texelSizeInput.y);

	//Sample 3x3 neighborhood around current pixel
	vec4 nbrSamples[9];
	nbrSamples[0] = FetchSampleValue(inputTexture, texCoordsFrag - texelSizeInput);
	nbrSamples[1] = FetchSampleValue(inputTexture, texCoordsFrag - texelSizeInputY);
	nbrSamples[2] = FetchSampleValue(inputTexture, texCoordsFrag + texelSizeInputX - texelSizeInputY);
	nbrSamples[3] = FetchSampleValue(inputTexture, texCoordsFrag - texelSizeInputX);
	nbrSamples[4] = currentPixelValue;
	nbrSamples[5] = FetchSampleValue(inputTexture, texCoordsFrag + texelSizeInputX);
	nbrSamples[6] = FetchSampleValue(inputTexture, texCoordsFrag - texelSizeInputX + texelSizeInputY);
	nbrSamples[7] = FetchSampleValue(inputTexture, texCoordsFrag + texelSizeInputY);
	nbrSamples[8] = FetchSampleValue(inputTexture, texCoordsFrag + texelSizeInput);

	vec4 neighborhoodMoment  = vec4(0.0f);
	vec4 neighborhoodMoment2 = vec4(0.0f);
	for(uint i = 0; i < 9; i++)
	{
		neighborhoodMoment  += nbrSamples[i];
		neighborhoodMoment2 += nbrSamples[i] * nbrSamples[i];
	}

	vec4 neighborhoodMu  = neighborhoodMoment  / 9.0f;
	vec4 neighborhoodMu2 = neighborhoodMoment2 / 9.0f;
	vec4 neighborhoodMin = nbrSamples[0];
	vec4 neighborhoodMax = nbrSamples[0];
	for(uint i = 1; i < 9; i++)
	{
		neighborhoodMin = min(neighborhoodMin, nbrSamples[i]);
		neighborhoodMax = max(neighborhoodMax, nbrSamples[i]);
	}
	
	//Cross (+) shape neighborhood values
	vec4 neighborhoodCrossMoment  = nbrSamples[1] + nbrSamples[3] + nbrSamples[4] + nbrSamples[5] + nbrSamples[7];
	vec4 neighborhoodCrossMoment2 = nbrSamples[1] * nbrSamples[1] + nbrSamples[3] * nbrSamples[3] + nbrSamples[4] * nbrSamples[4] + nbrSamples[5] * nbrSamples[5] + nbrSamples[7] * nbrSamples[7];
	vec4 neighborhoodCrossMu      = neighborhoodCrossMoment  / 5.0f;
	vec4 neighborhoodCrossMu2     = neighborhoodCrossMoment2 / 5.0f;
	vec4 neighborhoodCrossMin     = min(nbrSamples[1], min(nbrSamples[3], min(nbrSamples[4], min(nbrSamples[5], nbrSamples[7]))));
	vec4 neighborhoodCrossMax     = max(nbrSamples[1], max(nbrSamples[3], max(nbrSamples[4], max(nbrSamples[5], nbrSamples[7]))));

	//https://de45xmedrsdbp.cloudfront.net/Resources/files/TemporalAA_small-59732822.pdf Slide 32
	//Blend cross neighborhood values and 3x3 neighborhood values to get soft corners
	//NOTE: Taking a simple average of mu's may not be the mathematically correct way to blend them
	neighborhoodMu  = (neighborhoodMu  + neighborhoodCrossMu)  * 0.5f;
	neighborhoodMu2 = (neighborhoodMu2 + neighborhoodCrossMu2) * 0.5f;
	neighborhoodMin = (neighborhoodMin + neighborhoodCrossMin) * 0.5f;
	neighborhoodMax = (neighborhoodMax + neighborhoodCrossMax) * 0.5f;

	//Clip previous pixel value to current local neighborhood's color space AABB
	//Chrominance tightening disabled until a solution for shrinking mu2 is found. Though maybe it's not necessary and tightening only the min/max is better?
	#if(false)
	TightenAABBchroma(neighborhoodMu.xyz, neighborhoodMu2.xyz, neighborhoodMin.xyz, neighborhoodMax.xyz, currentPixelValue.xyz);
	#endif
	//NOTE: Filtering the color moments (eg. with separable guassian, similar to exponential shadow maps) and using them to generate AABB could be better
	VarianceAABB(neighborhoodMin.xyz, neighborhoodMax.xyz, previousPixelValue.xyz, neighborhoodMu.xyz, neighborhoodMu2.xyz);
	previousPixelValue.xyz = ClipAABB(neighborhoodMin.xyz, neighborhoodMax.xyz, previousPixelValue.xyz);

	float modulationWeight = UnbiasedLuminanceWeight(currentPixelValue.x, previousPixelValue.x, neighborhoodMax.xyz);

	//TODO: Detect disocclusion: minimum depth in a 3x3 pixel neighborhood of the current frame is compared with the maximum depth in a corresponding 
	//2x2 pixel region in the previous frame. A disocclusion event is detected when the current depth exceeds the previous depth

	//Fallback to current image if reprojected coordinates are outside screen
	if(any(greaterThan(previousTexCoords, vec2(1.0f))) || any(lessThan(previousTexCoords, vec2(0.0f))))
		modulationWeight = 0.0f;

	vec4 resultValue = mix(currentPixelValue, previousPixelValue, modulationWeight);

	resultValue.rgb = YCOCGtoRGB(resultValue.xyz);
	resultValue.rgb = InverseTonemap(resultValue.rgb);
	return resultValue;
}