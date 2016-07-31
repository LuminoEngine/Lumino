
#pragma once

LN_NAMESPACE_BEGIN

class PerlinNoise
{
public:

	PerlinNoise(unsigned int seed = 1);

	float Noise(float x) const;

	float Noise(float x, float y) const;

	float Noise(float x, float y, float z) const;

	float OctaveNoise(float x, int octaves) const;

	float OctaveNoise(float x, float y, int octaves) const;

	float OctaveNoise(float x, float y, float z, int octaves) const;

private:
	float Fade(float t) const;
	float Lerp(float t, float a, float b) const;
	float Grad(int hash, float x, float y, float z) const;

	int p[512];
};


LN_NAMESPACE_END

