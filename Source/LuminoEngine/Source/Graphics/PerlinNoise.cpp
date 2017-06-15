#include <cmath>
#include <array>
#include <numeric>
#include <random>
#include <algorithm>
#include "Internal.h"
#include <Lumino/Graphics/PerlinNoise.h>

LN_NAMESPACE_BEGIN

static const float persistence = 0.5f;

//------------------------------------------------------------------------------
PerlinNoise::PerlinNoise()
	: m_repeat(0)
{
	setSeed((int)::time(NULL));
}

//------------------------------------------------------------------------------
PerlinNoise::PerlinNoise(int seed)
	: m_repeat(0)
{
	setSeed(seed);
}

//------------------------------------------------------------------------------
void PerlinNoise::setSeed(int seed)
{
	// p[0..255] を埋めてシャッフル
	for (int i = 0; i < 256; ++i) p[i] = i;
	std::shuffle(std::begin(p), std::begin(p) + 256, std::mt19937(seed));

	for (int i = 0; i<256; ++i)
	{
		p[256 + i] = p[i];
	}
}

//------------------------------------------------------------------------------
void PerlinNoise::setTiledRepeatFrequency(float repeat)
{
	m_repeat = repeat;
}

//------------------------------------------------------------------------------
float PerlinNoise::noise1D(float x, int octaves, float minValue, float maxValue) const
{
	float v = octaveNoiseNormal(x, 0, 0, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f) * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::noise2D(float x, float y, int octaves, float minValue, float maxValue) const
{
	float v = octaveNoiseNormal(x, y, 0, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f)  * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::noise3D(float x, float y, float z, int octaves, float minValue, float maxValue) const
{
	float v = octaveNoiseNormal(x, y, z, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f)  * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::noiseNormal(float x, float y, float z) const
{
	if (m_repeat > 0)
	{
		x = fmod(x, m_repeat);
		y = fmod(y, m_repeat);
		z = fmod(z, m_repeat);
	}

	int xi = (int)x & 255;
	int yi = (int)y & 255;
	int zi = (int)z & 255;
	float xf = x - (int)x;
	float yf = y - (int)y;
	float zf = z - (int)z;
	float u = fade(xf);
	float v = fade(yf);
	float w = fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + increment(yi)] + zi];
	aab = p[p[p[xi] + yi] + increment(zi)];
	abb = p[p[p[xi] + increment(yi)] + increment(zi)];
	baa = p[p[p[increment(xi)] + yi] + zi];
	bba = p[p[p[increment(xi)] + increment(yi)] + zi];
	bab = p[p[p[increment(xi)] + yi] + increment(zi)];
	bbb = p[p[p[increment(xi)] + increment(yi)] + increment(zi)];

	float x1, x2, y1, y2;
	x1 = lerp(grad(aaa, xf, yf, zf),
		grad(baa, xf - 1, yf, zf),
		u);
	x2 = lerp(grad(aba, xf, yf - 1, zf),
		grad(bba, xf - 1, yf - 1, zf),
		u);
	y1 = lerp(x1, x2, v);

	x1 = lerp(grad(aab, xf, yf, zf - 1),
		grad(bab, xf - 1, yf, zf - 1),
		u);
	x2 = lerp(grad(abb, xf, yf - 1, zf - 1),
		grad(bbb, xf - 1, yf - 1, zf - 1),
		u);
	y2 = lerp(x1, x2, v);

	return lerp(y1, y2, w);	// -1.0 .. 1.0
}

//------------------------------------------------------------------------------
float PerlinNoise::octaveNoiseNormal(float x, float y, float z, int octaves) const
{
	float result = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	for (int i = 0; i < octaves; ++i)
	{
		result += noiseNormal(x * frequency, y * frequency, z * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return result;	// -1.0 .. 1.0
}

//------------------------------------------------------------------------------
float PerlinNoise::fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

//------------------------------------------------------------------------------
float PerlinNoise::lerp(float a, float b, float t) const
{
	return a + t * (b - a);
}

//------------------------------------------------------------------------------
float PerlinNoise::grad(int hash, float x, float y, float z) const
{
	int h = hash & 15;
	float u = (h < 8) ? x : y;
	float v = (h < 4) ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//------------------------------------------------------------------------------
int PerlinNoise::increment(int num) const
{
	num++;
	if (m_repeat > 0) num %= m_repeat;
	return num;
}

LN_NAMESPACE_END
