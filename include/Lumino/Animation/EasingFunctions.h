
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace Lumino
{

typedef std::function< float(float, float, float, float) >		FloatEasingFunction;
typedef std::function< float(double, double, double, double) >	DoubleEasingFunction;


/// 各関数の引数は、t:現在時間(0.0～d) b:開始値 c:値の変化量 (目標値-開始値) d:変化にかける時間
class EasingFunctions
{
public:
	/// simple linear tweening - no easing, no acceleration 
	template<typename TValue, typename TTime>
	static TValue LinearTween(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (t / d) + b;
	};

	/// quadratic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t + b;
	};

	/// quadratic easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * t*(t - 2) + b;
	};

	/// quadratic easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue EaseInOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t + b;
		t--;
		return -c / 2 * (t*(t - 2) - 1) + b;
	};

	/// cubic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t + b;
	};

	/// cubic easing out - decelerating to zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t + 1) + b;
	};

	/// cubic easing in/out - acceleration until halfway, then deceleration 
	template<typename TValue, typename TTime>
	static TValue EaseInOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t + 2) + b;
	};

	/// quartic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t + b;
	};

	/// quartic easing out - decelerating to zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return -c * (t*t*t*t - 1) + b;
	};

	/// quartic easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue EaseInOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t + b;
		t -= 2;
		return -c / 2 * (t*t*t*t - 2) + b;
	};

	/// quintic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t*t + b;
	};

	/// quintic easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t*t*t + 1) + b;
	};

	/// quintic easing in/out - acceleration until halfway, then deceleration 
	template<typename TValue, typename TTime>
	static TValue EaseInOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t*t*t + 2) + b;
	};

	/// sinusoidal easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c * std::cos(t / d * (M_PI / 2.0)) + c + b;
	};

	/// sinusoidal easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::sin(t / d * (M_PI / 2.0)) + b;
	};

	/// sinusoidal easing in/out - accelerating until halfway, then decelerating
	template<typename TValue, typename TTime>
	static TValue EaseInOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c / 2.0 * (std::cos(M_PI*t / d) - 1) + b;
	};

	/// exponential easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue EaseInExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::pow(2.0, 10.0 * (t / d - 1)) + b;
	};

	/// exponential easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (-std::pow(2, -10 * t / d) + 1) + b;
	};

	/// exponential easing in/out - accelerating until halfway, then decelerating 
	template<typename TValue, typename TTime>
	static TValue EaseInOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * std::pow(2, 10 * (t - 1)) + b;
		t--;
		return c / 2 * (-std::pow(2, -10 * t) + 2) + b;
	};

	/// circular easing in - accelerating from zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseInCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * (std::sqrt(1 - t*t) - 1) + b;
	};

	/// circular easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue EaseOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c * std::sqrt(1 - t*t) + b;
	};

	/// circular easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue EaseInOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return -c / 2 * (std::sqrt(1 - t*t) - 1) + b;
		t -= 2;
		return c / 2 * (std::sqrt(1 - t*t) + 1) + b;
	};
};

} // namespace Lumino
