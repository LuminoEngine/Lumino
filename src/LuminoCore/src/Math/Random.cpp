
#include <time.h>
#include <Lumino/Math/Random.hpp>

namespace ln {

//==============================================================================
// Random

#define DEFAULT_X 123456789
#define DEFAULT_Y 362436069
#define DEFAULT_Z 521288629
#define DEFAULT_W 88675123

Random::Random()
{
    setSeed((int)::time(NULL));
}

Random::Random(int seed)
{
    setSeed(seed);
}

void Random::setSeed(int seed)
{
    x = DEFAULT_X;
    y = DEFAULT_Y;
    z = DEFAULT_Z;
    w = (seed <= 0) ? 88675123 : seed;
}

int Random::getInt()
{
    unsigned t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    return static_cast<int>(w & 0x7fffffff); // 符号を消す
}

int Random::getInt(int maxValue)
{
    if (maxValue == 0) {
        return 0;
    }
    int r = getInt();
    r %= maxValue;
    return r;
}

int Random::getIntRange(int minValue, int maxValue)
{
    if (maxValue - minValue == 0) {
        return 0;
    }
    int r = getInt();
    r %= maxValue - minValue;
    r += minValue;
    return r;
}

int Random::getIntWidth(int median, int width)
{
    return getIntRange(median - width, median + width);
}

float Random::getFloat()
{
    int r = getInt();
    r = (r & 0x007fffff) | 0x3f800000; // 0x3f800000 は指数部が 1111111で、これは指数1を示す。r & 0x007fffff で仮数部を適当に決めている。
    float f = *reinterpret_cast<float*>(&r);
    return f - 1.f;
}

float Random::getFloat(float maxValue)
{
    float r = getFloat();
    r *= maxValue;
    return r;
}

float Random::getFloatRange(float minValue, float maxValue)
{
    float r = getFloat();
    r *= (maxValue - minValue);
    r += minValue;
    return r;
}

float Random::getFloatWidth(float median, float width)
{
    return getFloatRange(median - width, median + width);
}

} // namespace ln
