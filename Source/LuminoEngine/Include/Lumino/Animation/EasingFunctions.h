
#pragma once
#include <cmath>

LN_NAMESPACE_BEGIN

typedef std::function< float(float, float, float, float) >		FloatEasingFunction;
typedef std::function< float(double, double, double, double) >	DoubleEasingFunction;


/// 各関数の引数は、t:現在時間(0.0～d) b:開始値 c:値の変化量 (目標値-開始値) d:変化にかける時間
class EasingFunctions
{
public:
	/// simple linear tweening - no easing, no acceleration 
	template<typename TValue, typename TTime>
	static TValue linearTween(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (t / d) + b;
	};

	/// quadratic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t + b;
	};

	/// quadratic easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue easeOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * t*(t - 2) + b;
	};

	/// quadratic easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue easeInOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t + b;
		t--;
		return -c / 2 * (t*(t - 2) - 1) + b;
	};

	/// cubic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t + b;
	};

	/// cubic easing out - decelerating to zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t + 1) + b;
	};

	/// cubic easing in/out - acceleration until halfway, then deceleration 
	template<typename TValue, typename TTime>
	static TValue easeInOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t + 2) + b;
	};

	/// quartic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t + b;
	};

	/// quartic easing out - decelerating to zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return -c * (t*t*t*t - 1) + b;
	};

	/// quartic easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue easeInOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t + b;
		t -= 2;
		return -c / 2 * (t*t*t*t - 2) + b;
	};

	/// quintic easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t*t + b;
	};

	/// quintic easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue easeOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t*t*t + 1) + b;
	};

	/// quintic easing in/out - acceleration until halfway, then deceleration 
	template<typename TValue, typename TTime>
	static TValue easeInOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t*t*t + 2) + b;
	};

	/// sinusoidal easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c * std::cos(t / d * ((3.14159265358979323846f) / 2.0f)) + c + b;
	};

	/// sinusoidal easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue easeOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::sin(t / d * ((3.14159265358979323846f) / 2.0f)) + b;
	};

	/// sinusoidal easing in/out - accelerating until halfway, then decelerating
	template<typename TValue, typename TTime>
	static TValue easeInOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c / 2.0 * (std::cos((3.14159265358979323846f) *t / d) - 1) + b;
	};

	/// exponential easing in - accelerating from zero velocity 
	template<typename TValue, typename TTime>
	static TValue easeInExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::pow(2.0, 10.0 * (t / d - 1)) + b;
	};

	/// exponential easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue easeOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (-std::pow(2, -10 * t / d) + 1) + b;
	};

	/// exponential easing in/out - accelerating until halfway, then decelerating 
	template<typename TValue, typename TTime>
	static TValue easeInOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * std::pow(2, 10 * (t - 1)) + b;
		t--;
		return c / 2 * (-std::pow(2, -10 * t) + 2) + b;
	};

	/// circular easing in - accelerating from zero velocity
	template<typename TValue, typename TTime>
	static TValue easeInCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * (std::sqrt(1 - t*t) - 1) + b;
	};

	/// circular easing out - decelerating to zero velocity
	template<typename TValue, typename TTime>
	static TValue easeOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c * std::sqrt(1 - t*t) + b;
	};

	/// circular easing in/out - acceleration until halfway, then deceleration
	template<typename TValue, typename TTime>
	static TValue easeInOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return -c / 2 * (std::sqrt(1 - t*t) - 1) + b;
		t -= 2;
		return c / 2 * (std::sqrt(1 - t*t) + 1) + b;
	};
};

LN_NAMESPACE_END
