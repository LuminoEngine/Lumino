
#include <math.h>
#include <float.h>
#include <limits>
#include <LuminoCore/Math/Math.hpp>

namespace ln {

//==============================================================================
// Math

const float Math::NaN = std::numeric_limits<float>::quiet_NaN();
const float Math::Inf = std::numeric_limits<float>::infinity();
const float Math::PI = 3.1415926535897932384626433832795f;
const float Math::PI2 = 6.283185307f;
const float Math::PIDiv2 = 1.570796327f;

unsigned int Math::nextPow2(unsigned int value)
{
    unsigned int i = 1;
    while (value > (i <<= 1)) {
        if (!i)
            break;
    }
    return i;
}

void Math::sinCos(float value, float* outSin, float* outCos)
{
    // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
    float quotient = (1.0f / PI2) * value;
    if (value >= 0.0f) {
        quotient = (float)((int)(quotient + 0.5f));
    } else {
        quotient = (float)((int)(quotient - 0.5f));
    }
    float y = value - PI2 * quotient;

    // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
    float sign;
    if (y > PIDiv2) {
        y = PI - y;
        sign = -1.0f;
    } else if (y < -PIDiv2) {
        y = -PI - y;
        sign = -1.0f;
    } else {
        sign = +1.0f;
    }

    float y2 = y * y;

    // 11-degree minimax approximation
    *outSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

    // 10-degree minimax approximation
    float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
    *outCos = sign * p;
}

bool Math::isNaN(float value)
{
#ifdef _MSC_VER
    return (_isnan(value) != 0);
#else
    return (isnan(value) != 0);
#endif
}

bool Math::isNaN(double value)
{
#ifdef _MSC_VER
    return (_isnan(value) != 0);
#else
    return (isnan(value) != 0);
#endif
}

bool Math::isInf(float value)
{
#ifdef _MSC_VER
    return (_isnan(value) == 0 && _finite(value) == 0);
#else
    return (isinf(value) != 0);
#endif
}

bool Math::isInf(double value)
{
#ifdef _MSC_VER
    return (_isnan(value) == 0 && _finite(value) == 0);
#else
    return (isinf(value) != 0);
#endif
}

} // namespace ln
