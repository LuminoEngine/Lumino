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
	SetSeed((int)::time(NULL));
}

//------------------------------------------------------------------------------
PerlinNoise::PerlinNoise(int seed)
	: m_repeat(0)
{
	SetSeed(seed);
}

//------------------------------------------------------------------------------
void PerlinNoise::SetSeed(int seed)
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
void PerlinNoise::SetTiledRepeatFrequency(float repeat)
{
	m_repeat = repeat;
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise1D(float x, int octaves, float minValue, float maxValue) const
{
	float v = OctaveNoiseNormal(x, 0, 0, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f) * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise2D(float x, float y, int octaves, float minValue, float maxValue) const
{
	float v = OctaveNoiseNormal(x, y, 0, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f)  * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::Noise3D(float x, float y, float z, int octaves, float minValue, float maxValue) const
{
	float v = OctaveNoiseNormal(x, y, z, octaves);
	return Math::clamp(((v + 1.0f) / 2.0f)  * (maxValue - minValue) + minValue, minValue, maxValue);
}

//------------------------------------------------------------------------------
float PerlinNoise::NoiseNormal(float x, float y, float z) const
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
	float u = Fade(xf);
	float v = Fade(yf);
	float w = Fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + Increment(yi)] + zi];
	aab = p[p[p[xi] + yi] + Increment(zi)];
	abb = p[p[p[xi] + Increment(yi)] + Increment(zi)];
	baa = p[p[p[Increment(xi)] + yi] + zi];
	bba = p[p[p[Increment(xi)] + Increment(yi)] + zi];
	bab = p[p[p[Increment(xi)] + yi] + Increment(zi)];
	bbb = p[p[p[Increment(xi)] + Increment(yi)] + Increment(zi)];

	float x1, x2, y1, y2;
	x1 = lerp(Grad(aaa, xf, yf, zf),
		Grad(baa, xf - 1, yf, zf),
		u);
	x2 = lerp(Grad(aba, xf, yf - 1, zf),
		Grad(bba, xf - 1, yf - 1, zf),
		u);
	y1 = lerp(x1, x2, v);

	x1 = lerp(Grad(aab, xf, yf, zf - 1),
		Grad(bab, xf - 1, yf, zf - 1),
		u);
	x2 = lerp(Grad(abb, xf, yf - 1, zf - 1),
		Grad(bbb, xf - 1, yf - 1, zf - 1),
		u);
	y2 = lerp(x1, x2, v);

	return lerp(y1, y2, w);	// -1.0 .. 1.0
}

//------------------------------------------------------------------------------
float PerlinNoise::OctaveNoiseNormal(float x, float y, float z, int octaves) const
{
	float result = 0.0;
	float frequency = 1.0;
	float amplitude = 1.0;
	for (int i = 0; i < octaves; ++i)
	{
		result += NoiseNormal(x * frequency, y * frequency, z * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return result;	// -1.0 .. 1.0
}

//------------------------------------------------------------------------------
float PerlinNoise::Fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

//------------------------------------------------------------------------------
float PerlinNoise::lerp(float a, float b, float t) const
{
	return a + t * (b - a);
}

//------------------------------------------------------------------------------
float PerlinNoise::Grad(int hash, float x, float y, float z) const
{
	int h = hash & 15;
	float u = (h < 8) ? x : y;
	float v = (h < 4) ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//------------------------------------------------------------------------------
int PerlinNoise::Increment(int num) const
{
	num++;
	if (m_repeat > 0) num %= m_repeat;
	return num;
}

LN_NAMESPACE_END
