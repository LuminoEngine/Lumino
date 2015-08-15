
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace Lumino
{

/// 各関数の引数は、t:時間(0.0～duration) b:開始値 c:値の変化量 (目標値-開始値) d:変化にかける時間
class EasingFunctions
{
public:
	/// simple linear tweening - no easing, no acceleration 
	template<typename T>
	static T LinearTween(T t, T b, T c, T d)
	{
		return c * (t / d) + b;
	};

	/// quadratic easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInQuad(T t, T b, T c, T d)
	{
		t /= d;
		return c*t*t + b;
	};

	/// quadratic easing out - decelerating to zero velocity
	template<typename T>
	static T EaseOutQuad(T t, T b, T c, T d)
	{
		t /= d;
		return -c * t*(t - 2) + b;
	};

	/// quadratic easing in/out - acceleration until halfway, then deceleration
	template<typename T>
	static T EaseInOutQuad(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t + b;
		t--;
		return -c / 2 * (t*(t - 2) - 1) + b;
	};

	/// cubic easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInCubic(T t, T b, T c, T d)
	{
		t /= d;
		return c*t*t*t + b;
	};

	/// cubic easing out - decelerating to zero velocity 
	template<typename T>
	static T EaseOutCubic(T t, T b, T c, T d)
	{
		t /= d;
		t--;
		return c*(t*t*t + 1) + b;
	};

	/// cubic easing in/out - acceleration until halfway, then deceleration 
	template<typename T>
	static T EaseInOutCubic(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t + 2) + b;
	};

	/// quartic easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInQuart(T t, T b, T c, T d)
	{
		t /= d;
		return c*t*t*t*t + b;
	};

	/// quartic easing out - decelerating to zero velocity 
	template<typename T>
	static T EaseOutQuart(T t, T b, T c, T d)
	{
		t /= d;
		t--;
		return -c * (t*t*t*t - 1) + b;
	};

	/// quartic easing in/out - acceleration until halfway, then deceleration
	template<typename T>
	static T EaseInOutQuart(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t + b;
		t -= 2;
		return -c / 2 * (t*t*t*t - 2) + b;
	};

	/// quintic easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInQuint(T t, T b, T c, T d)
	{
		t /= d;
		return c*t*t*t*t*t + b;
	};

	/// quintic easing out - decelerating to zero velocity
	template<typename T>
	static T EaseOutQuint(T t, T b, T c, T d)
	{
		t /= d;
		t--;
		return c*(t*t*t*t*t + 1) + b;
	};

	/// quintic easing in/out - acceleration until halfway, then deceleration 
	template<typename T>
	static T EaseInOutQuint(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t*t*t + 2) + b;
	};

	/// sinusoidal easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInSine(T t, T b, T c, T d)
	{
		return -c * std::cos(t / d * (M_PI / 2.0)) + c + b;
	};

	/// sinusoidal easing out - decelerating to zero velocity
	template<typename T>
	static T EaseOutSine(T t, T b, T c, T d)
	{
		return c * std::sin(t / d * (M_PI / 2.0)) + b;
	};

	/// sinusoidal easing in/out - accelerating until halfway, then decelerating
	template<typename T>
	static T EaseInOutSine(T t, T b, T c, T d)
	{
		return -c / 2.0 * (std::cos(M_PI*t / d) - 1) + b;
	};

	/// exponential easing in - accelerating from zero velocity 
	template<typename T>
	static T EaseInExpo(T t, T b, T c, T d)
	{
		return c * std::pow(2.0, 10.0 * (t / d - 1)) + b;
	};

	/// exponential easing out - decelerating to zero velocity
	template<typename T>
	static T EaseOutExpo(T t, T b, T c, T d)
	{
		return c * (-Math.pow(2, -10 * t / d) + 1) + b;
	};

	/// exponential easing in/out - accelerating until halfway, then decelerating 
	template<typename T>
	static T EaseInOutExpo(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * Math.pow(2, 10 * (t - 1)) + b;
		t--;
		return c / 2 * (-std::pow(2, -10 * t) + 2) + b;
	};

	/// circular easing in - accelerating from zero velocity
	template<typename T>
	static T EaseInCirc(T t, T b, T c, T d)
	{
		t /= d;
		return -c * (std::sqrt(1 - t*t) - 1) + b;
	};

	/// circular easing out - decelerating to zero velocity
	template<typename T>
	static T EaseOutCirc(T t, T b, T c, T d)
	{
		t /= d;
		t--;
		return c * std::sqrt(1 - t*t) + b;
	};

	/// circular easing in/out - acceleration until halfway, then deceleration
	template<typename T>
	static T EaseInOutCirc(T t, T b, T c, T d)
	{
		t /= d / 2;
		if (t < 1) return -c / 2 * (std::sqrt(1 - t*t) - 1) + b;
		t -= 2;
		return c / 2 * (std::sqrt(1 - t*t) + 1) + b;
	};
};

} // namespace Lumino
