#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "UIElement.hpp"

namespace ln {
class AnimationCurve;
	//enum class UIAnimationDirection
	//{
	//	/** 普通方向の再生でアニメーションサイクルを繰り返す。 */
	//	Normal,

	//	/** 奇数回では普通方向の再生、偶数回では逆方向の再生となって、アニメーションサイクルを繰り返す */
	//	Alternate,
	//};




class UIScalarAnimationTrack
	: public Object
{
public:
	bool testActiveTime(float time) const
	{
		return 0.0f <= time && time <= m_duration;
	}

	float evaluate(float time) const
	{
		return m_function(time, m_startValue, m_targetValue - m_startValue, m_duration);
	}

LN_CONSTRUCT_ACCESS:
	UIScalarAnimationTrack()
	{}
	bool init(float startValue, float targetValue, float duration, EasingMode func, float delay)
	{
		if (!Object::init()) return false;
		m_function = EasingFunctions::function(func);
		m_targetValue = targetValue;
		m_duration = duration;
		m_delay = delay;
		return true;
	}

private:
	EasingFunctions::FloatEasingFunctionPtr m_function;
	float m_startValue;
	float m_targetValue;
	float m_duration;
	float m_delay;
};

class UIVector4AnimationTrack
	: public Object
{
public:
	bool testActiveTime(float time) const
	{
		return 0.0f <= time && time <= m_duration;
	}

	Vector4 evaluate(float time) const
	{
		return m_function(time, m_startValue, m_targetValue - m_startValue, m_duration);
	}

LN_CONSTRUCT_ACCESS:
	UIVector4AnimationTrack()
	{}
	bool init(Vector4 startValue, Vector4 targetValue, float duration, EasingMode func, float delay)
	{
		if (!Object::init()) return false;
		m_function = EasingFunctions::functionVector4(func);
		m_targetValue = targetValue;
		m_duration = duration;
		m_delay = delay;
		return true;
	}

private:
	EasingFunctions::Vector4EasingFunctionPtr m_function;
	Vector4 m_startValue;
	Vector4 m_targetValue;
	float m_duration;
	float m_delay;
};

class UIScalarTransitionTrack
	: public Object
{
public:
	bool testActiveTime(float time) const
	{
		return 0.0f <= time && time <= m_duration;
	}

	float evaluate(float startValue, float time) const
	{
		return m_function(time, startValue, m_targetValue - startValue, m_duration);
	}

LN_CONSTRUCT_ACCESS:
	UIScalarTransitionTrack()
	{}
	bool init(float targetValue, float duration, EasingMode func, float delay)
	{
		if (!Object::init()) return false;
		m_function = EasingFunctions::function(func);
		m_targetValue = targetValue;
		m_duration = duration;
		m_delay = delay;
		return true;
	}

private:
	EasingFunctions::FloatEasingFunctionPtr m_function;
	float m_targetValue;
	float m_duration;
	float m_delay;
};

class UIVector4TransitionTrack
	: public Object
{
public:
	bool testActiveTime(float time) const
	{
		return 0.0f <= time && time <= m_duration;
	}

	Vector4 evaluate(Vector4 startValue, float time)  const
	{
		return m_function(time, startValue, m_targetValue - startValue, m_duration);
	}

LN_CONSTRUCT_ACCESS:
	UIVector4TransitionTrack()
	{}
	bool init(Vector4 targetValue, float duration, EasingMode func, float delay)
	{
		if (!Object::init()) return false;
		m_function = EasingFunctions::functionVector4(func);
		m_targetValue = targetValue;
		m_duration = duration;
		m_delay = delay;
		return true;
	}

private:
	EasingFunctions::Vector4EasingFunctionPtr m_function;
	Vector4 m_targetValue;
	float m_duration;
	float m_delay;
};


//class UIAnimationTrack
//	: public Object
//{
//public:
//
//LN_CONSTRUCT_ACCESS:
//	UIAnimationTrack();
//	bool init();
//
//private:
//
//};
//

namespace detail {
class UIStyleInstance;

enum UIStyleAnimationElement
{
	UIStyleAnimationElement_Width,
	UIStyleAnimationElement_Height,
	UIStyleAnimationElement_PositionX,
	UIStyleAnimationElement_PositionY,
	UIStyleAnimationElement_PositionZ,
	UIStyleAnimationElement_BackgroundColor,
	UIStyleAnimationElement_Opacity,

	UIStyleAnimationElement_Count_,
};

// finalStyle に付く
class UIScalarAnimationInstance
	: public RefObject
{
public:
	void advanceTime(float elapsedTimer)
	{
		m_time += elapsedTimer;
	}

	bool isActive() const
	{
		if (m_transitionTrack)
			return m_transitionTrack->testActiveTime(m_time);
		else
			return m_animationTrack->testActiveTime(m_time);
	}

	float evaluate() const
	{
		if (m_transitionTrack)
			return m_transitionTrack->evaluate(m_startValue, m_time);
		else
			return m_animationTrack->evaluate(m_time);
	}

public:
	// m_animationTrack と m_transitionTrack はどちらかひとつだけもつ。
	// union にしてもそんなにオブジェクトサイズ稼げないのでこのままにしてみる。
	UIScalarAnimationTrack* m_animationTrack = nullptr;
	UIScalarTransitionTrack* m_transitionTrack = nullptr;
	float m_startValue;
	float m_time = 0.0f;
};

class UIVector4AnimationInstance
	: public RefObject
{
public:
	void advanceTime(float elapsedTimer)
	{
		m_time += elapsedTimer;
	}

	bool isActive() const
	{
		if (m_transitionTrack)
			return m_transitionTrack->testActiveTime(m_time);
		else
			return m_animationTrack->testActiveTime(m_time);
	}

	Vector4 evaluate() const
	{
		if (m_transitionTrack)
			return m_transitionTrack->evaluate(m_startValue, m_time);
		else
			return m_animationTrack->evaluate(m_time);
	}

public:
	UIVector4AnimationTrack* m_animationTrack = nullptr;
	UIVector4TransitionTrack* m_transitionTrack = nullptr;
	Vector4 m_startValue;
	float m_time = 0.0f;
};

} // namespace detail



class UIAnimationClip
	: public Object
{
public:
	void addWidth(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Width, startValue, targetValue, duration, func, wrapMode); }
	void addHeight(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Height, startValue, targetValue, duration, func, wrapMode); }
	void addPositionX(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionX, startValue, targetValue, duration, func, wrapMode); }
	void addPositionY(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionY, startValue, targetValue, duration, func, wrapMode); }
	void addPositionZ(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionZ, startValue, targetValue, duration, func, wrapMode); }
	void addOpacity(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Opacity, startValue, targetValue, duration, func, wrapMode); }

LN_CONSTRUCT_ACCESS:
	UIAnimationClip();
	bool init();

private:
	void addCurve(detail::UIStyleAnimationElement element, float startValue, float targetValue, float duration, EasingMode func, AnimationWrapMode wrapMode);

	std::array<Ref<AnimationCurve>, detail::UIStyleAnimationElement_Count_> m_curves;
};

class UITransitionClip
	: public Object
{
public:
	struct Curve
	{
		EasingFunctions::FloatEasingFunctionPtr function = nullptr;
		float targetValue;
		float duration;
		float delay;
	};

	void addWidth(float targetValue, float duration, EasingMode func = EasingMode::Linear, float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Width, targetValue, duration, func, delay); }
	void addHeight(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Height, targetValue, duration, func, delay); }
	void addPositionX(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionX, targetValue, duration, func, delay); }
	void addPositionY(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionY, targetValue, duration, func, delay); }
	void addPositionZ(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionZ, targetValue, duration, func, delay); }
	void addOpacity(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Opacity, targetValue, duration, func, delay); }

LN_CONSTRUCT_ACCESS:
	UITransitionClip();
    bool init();

private:
	void addCurve(detail::UIStyleAnimationElement element, float targetValue, float duration, EasingMode func, float delay);

	std::array<Curve, detail::UIStyleAnimationElement_Count_> m_curves;
};

} // namespace ln

