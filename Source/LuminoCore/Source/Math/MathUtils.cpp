
#include <math.h>
#include <float.h>
#include <limits>

// 標準ヘッダの math.h と被るので Utils を付けている
#include "../../include/Lumino/Math/MathUtils.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Math
//==============================================================================

const float Math::NaN		= std::numeric_limits<float>::quiet_NaN();
const float Math::Inf		= std::numeric_limits<float>::infinity();
const float Math::PI		= 3.1415926535897932384626433832795f;
const float Math::PI2		= 6.283185307f;
const float Math::PIDiv2	= 1.570796327f;

//------------------------------------------------------------------------------
unsigned int Math::nextPow2(unsigned int value)
{
	unsigned int i = 1;
	while (value > (i <<= 1)) {
		if (!i) break;
	}
	return i;
}

//------------------------------------------------------------------------------
void Math::sinCos(float value, float* outSin, float* outCos)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = (1.0f / PI2) * value;
	if (value >= 0.0f)
	{
		quotient = (float)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int)(quotient - 0.5f));
	}
	float y = value - PI2 * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > PIDiv2)
	{
		y = PI - y;
		sign = -1.0f;
	}
	else if (y < -PIDiv2)
	{
		y = -PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*outSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*outCos = sign*p;
}

//------------------------------------------------------------------------------
bool Math::nearEqual(float value1, float value2)
{
	return ((value2 - FLT_EPSILON) < value1) && (value1 < (value2 + FLT_EPSILON));
}

//------------------------------------------------------------------------------
bool Math::isNaN(float value)
{
#ifdef _MSC_VER
	return (_isnan(value) != 0);
#else
	return (isnan(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool Math::isNaN(double value)
{
#ifdef _MSC_VER
	return (_isnan(value) != 0);
#else
	return (isnan(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool Math::isInf(float value)
{
#ifdef _MSC_VER
	return (_isnan(value) == 0 && _finite(value) == 0);
#else
	return (isinf(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool Math::isInf(double value)
{
#ifdef _MSC_VER
	return (_isnan(value) == 0 && _finite(value) == 0);
#else
	return (isinf(value) != 0);
#endif
}

//------------------------------------------------------------------------------
float Math::lerp(float v1, float v2, float t)
{
	return v1 + ((v2 - v1) * t);
}

//------------------------------------------------------------------------------
float Math::quadAccel(float p, float v, float a, float t)
{
	return p + (v * t) + (0.5f * a * t * t);
}

//------------------------------------------------------------------------------
float Math::hermite(float v1, float a1, float v2, float a2, float t)
{
	float a = 2.f * (v1 - v2) + (a1 + a2);
	float b = 3.f * (v2 - v1) - (2.f * a1) - a2;
	float r = a;
	r *= t;
	r += b;
	r *= t;
	r += a1;
	r *= t;
	return r + v1;
}

//------------------------------------------------------------------------------
float Math::catmullRom(float v1, float v2, float v3, float v4, float t)
{
	float d1 = (v3 - v1) * 0.5f;
	float d2 = (v4 - v2) * 0.5f;
	return (2.0f*v2 - 2.0f*v3 + d1 + d2)*t*t*t + (-3.0f*v2 + 3.0f*v3 - 2.0f*d1 - d2)*t*t + d1*t + v2;
}

//------------------------------------------------------------------------------
float Math::cubicBezier(float v1, float v2, float v3, float v4, float t)
{
	float tp = 1.f - t;
	return t*t*t*v4 + 3.f*t*t*tp*v3 + 3.f*t*tp*tp*v2 + tp*tp*tp*v1;
}

LN_NAMESPACE_END
