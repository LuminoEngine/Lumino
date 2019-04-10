
#include <time.h>
#include <LuminoCore/Math/Random.hpp>

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

    // シンプルな Xorshift の実装では、seed を取ってからしばらくの間、上位ビットが同じような並びになる。
    // そのため、マクロな視点では似たような値が出力される。
    // これは floatValue() のような高速な float 乱数に直すと特に顕著で、小数第1位が毎回同じ値を返す。
    // 乱数で座標決めるような場合に大きな問題となる。
    // ここでは下位ビットを上位ビットに影響させることで、簡易的な対策を行っている。
    int v = ((w) ^ (w << 24) ^ (w << 16));
    return static_cast<int>(v & 0x7fffffff); // 符号を消す
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

float Random::floatValue()
{
    int r = getInt();
    
    //uint32_t m = (r & 0x007fffff) ^ (r >> 9/*(x & 0x7)*/);
    //uint32_t m = ((r) ^ (r >> 9) ^ (r << 16)) & 0x007fffff;
    r = (r & 0x007fffff) | 0x3f800000; // 0x3f800000 は指数部が 1111111で、これは指数1を示す。r & 0x007fffff で仮数部を適当に決めている。
    float f = *reinterpret_cast<float*>(&r);
    return f - 1.f;
}

float Random::floatValue(float maxValue)
{
    float r = floatValue();
    r *= maxValue;
    return r;
}

float Random::getFloatRange(float minValue, float maxValue)
{
    float r = floatValue();
    r *= (maxValue - minValue);
    r += minValue;
    return r;
}

float Random::getFloatWidth(float median, float width)
{
    return getFloatRange(median - width, median + width);
}

} // namespace ln
