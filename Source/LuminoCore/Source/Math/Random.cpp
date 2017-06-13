
#include <time.h>
#include "../../include/Lumino/Math/Random.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Randomizer
//==============================================================================

Randomizer Random;

#define DEFAULT_X 123456789
#define DEFAULT_Y 362436069
#define DEFAULT_Z 521288629
#define DEFAULT_W 88675123

//------------------------------------------------------------------------------
Randomizer::Randomizer()
{
	setSeed((int)::time(NULL));
}

//------------------------------------------------------------------------------
Randomizer::Randomizer(int seed)
{
	setSeed(seed);
}

//------------------------------------------------------------------------------
void Randomizer::setSeed(int seed)
{
	x = DEFAULT_X;
	y = DEFAULT_Y;
	z = DEFAULT_Z;
	w = (seed <= 0) ? 88675123 : seed;
}

//------------------------------------------------------------------------------
int Randomizer::getInt()
{
	unsigned t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return static_cast< int >(w & 0x7fffffff);	// 符号を消す
}

//------------------------------------------------------------------------------
int Randomizer::getInt(int maxValue)
{
	if (maxValue == 0) { return 0; }
	int r = getInt();
	r %= maxValue;
	return r;
}

//------------------------------------------------------------------------------
int Randomizer::getIntRange(int minValue, int maxValue)
{
	if (maxValue - minValue == 0) { return 0; }
	int r = getInt();
	r %= maxValue - minValue;
	r += minValue;
	return r;
}

//------------------------------------------------------------------------------
int Randomizer::getIntWidth(int median, int width)
{
	return getIntRange(median - width, median + width);
}

//------------------------------------------------------------------------------
float Randomizer::getFloat()
{
	int r = getInt();
	r = (r & 0x007fffff) | 0x3f800000;			// 0x3f800000 は指数部が 1111111で、これは指数1を示す。r & 0x007fffff で仮数部を適当に決めている。
	float f = *reinterpret_cast<float*>(&r);
	return f - 1.f;
}

//------------------------------------------------------------------------------
float Randomizer::getFloat(float maxValue)
{
	float r = getFloat();
	r *= maxValue;
	return r;
}

//------------------------------------------------------------------------------
float Randomizer::getFloatRange(float minValue, float maxValue)
{
	float r = getFloat();
	r *= (maxValue - minValue);
	r += minValue;
	return r;
}

//------------------------------------------------------------------------------
float Randomizer::getFloatWidth(float median, float width)
{
	return getFloatRange(median - width, median + width);
}

LN_NAMESPACE_END
