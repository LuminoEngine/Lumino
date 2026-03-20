#pragma once
#include <LuminoEngine/Animation/EasingFunctions.hpp>
#include <LuminoEngine/Animation/AnimationCurve.hpp>
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



class UIValueTrack
	: public Object
{
public:
	enum class ValueType
	{
		Float,
		Vector4,
	};
	enum class TrackType
	{
		Animation,
		Transition,
	};

	virtual ValueType valueType() const = 0;
	virtual TrackType trackType() const = 0;
	float duration() const { return m_duration; }
	float delay() const { return m_delay; }
	AnimationWrapMode wrapMode() const { return m_wrapMode; }
	bool testActiveTime(float time) const { return m_wrapMode != AnimationWrapMode::Once || 0.0f <= time && time <= m_duration; }

LN_CONSTRUCT_ACCESS:
	UIValueTrack()
	{}
	bool init(float duration, float delay, AnimationWrapMode wrapMode)
	{
		if (!Object::init()) return false;
		m_duration = duration;
		m_delay = delay;
		m_wrapMode = wrapMode;
		return true;
	}

private:
	float m_duration;
	float m_delay;
	AnimationWrapMode m_wrapMode;
};

class UIScalarAnimationTrack
	: public UIValueTrack
{
public:
	ValueType valueType() const override { return ValueType::Float; }
	TrackType trackType() const override { return TrackType::Animation; }
	float evaluate(float time) const
	{
		return m_function(
			AnimationCurve::calculateLocalTime(time, duration(), wrapMode()),
			m_startValue, m_targetValue - m_startValue, duration());
	}

LN_CONSTRUCT_ACCESS:
	UIScalarAnimationTrack()
	{}
	bool init(float startValue, float targetValue, float duration, EasingMode func, float delay, AnimationWrapMode wrapMode)
	{
		if (!UIValueTrack::init(duration, delay, wrapMode)) return false;
		m_function = EasingFunctions::function(func);
		m_startValue = startValue;
		m_targetValue = targetValue;
		return true;
	}

private:
	EasingFunctions::FloatEasingFunctionPtr m_function;
	float m_startValue;
	float m_targetValue;
};

class UIVector4AnimationTrack
	: public UIValueTrack
{
public:
	ValueType valueType() const override { return ValueType::Vector4; }
	TrackType trackType() const override { return TrackType::Animation; }
	Vector4 evaluate(float time) const
	{
		return m_function(
			AnimationCurve::calculateLocalTime(time, duration(), wrapMode()),
			m_startValue, m_targetValue - m_startValue, duration());
	}

LN_CONSTRUCT_ACCESS:
	UIVector4AnimationTrack()
	{}
	bool init(Vector4 startValue, Vector4 targetValue, float duration, EasingMode func, float delay, AnimationWrapMode wrapMode)
	{
		if (!UIValueTrack::init(duration, delay, wrapMode)) return false;
		m_function = EasingFunctions::functionVector4(func);
		m_startValue = startValue;
		m_targetValue = targetValue;
		return true;
	}

private:
	EasingFunctions::Vector4EasingFunctionPtr m_function;
	Vector4 m_startValue;
	Vector4 m_targetValue;
};

class UIScalarTransitionTrack
	: public UIValueTrack
{
public:
	ValueType valueType() const override { return ValueType::Float; }
	TrackType trackType() const override { return TrackType::Transition; }
	float evaluate(float startValue, float time) const
	{
		return m_function(
			AnimationCurve::calculateLocalTime(time, duration(), wrapMode()),
			startValue, m_targetValue - startValue, duration());
	}

LN_CONSTRUCT_ACCESS:
	UIScalarTransitionTrack()
	{}
	bool init(float targetValue, float duration, EasingMode func, float delay)
	{
		if (!UIValueTrack::init(duration, delay, AnimationWrapMode::Once)) return false;
		m_function = EasingFunctions::function(func);
		m_targetValue = targetValue;
		return true;
	}

private:
	EasingFunctions::FloatEasingFunctionPtr m_function;
	float m_targetValue;
};

class UIVector4TransitionTrack
	: public UIValueTrack
{
public:
	ValueType valueType() const override { return ValueType::Vector4; }
	TrackType trackType() const override { return TrackType::Transition; }
	Vector4 evaluate(Vector4 startValue, float time)  const
	{
		return m_function(
			AnimationCurve::calculateLocalTime(time, duration(), wrapMode()),
			startValue, m_targetValue - startValue, duration());
	}

LN_CONSTRUCT_ACCESS:
	UIVector4TransitionTrack()
	{}
	bool init(Vector4 targetValue, float duration, EasingMode func, float delay)
	{
		if (!UIValueTrack::init(duration, delay, AnimationWrapMode::Once)) return false;
		m_function = EasingFunctions::functionVector4(func);
		m_targetValue = targetValue;
		return true;
	}

private:
	EasingFunctions::Vector4EasingFunctionPtr m_function;
	Vector4 m_targetValue;
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

// finalStyle に付く.
// Instance 側は型は既知なのでそれを利用し、毎ループ実行されるようなものは
// できるだけ仮想関数使わないような実装にしたい。
class UIScalarAnimationInstance
	: public RefObject
{
public:
	static bool realloc(UIValueTrack* source, Ref<UIScalarAnimationInstance>* instance)
	{
		if ((*instance) && source == (*instance)->m_sourceTrack) {
			return false;
		}

		if (LN_REQUIRE(source->valueType() == UIValueTrack::ValueType::Float)) return false;

		if (!(*instance)) {
			(*instance) = makeRef<UIScalarAnimationInstance>();
		}

		if (source->trackType() == UIValueTrack::TrackType::Animation) {
			(*instance)->m_animationTrack = static_cast<UIScalarAnimationTrack*>(source);
		}
		else if (source->trackType() == UIValueTrack::TrackType::Transition) {
			(*instance)->m_transitionTrack = static_cast<UIScalarTransitionTrack*>(source);
		}
		else {
			LN_UNREACHABLE();
		}

		(*instance)->m_sourceTrack = source;
		return true;
	}

	void reset(float startValue)
	{
		m_startValue = startValue;
		m_time = 0.0f;
	}

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
	UIValueTrack* m_sourceTrack = nullptr;
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
	static bool realloc(UIValueTrack* source, Ref<UIVector4AnimationInstance>* instance)
	{
		if ((*instance) && source == (*instance)->m_sourceTrack) {
			return false;
		}

		if (LN_REQUIRE(source->valueType() == UIValueTrack::ValueType::Vector4)) return false;

		if (!(*instance)) {
			(*instance) = makeRef<UIVector4AnimationInstance>();
		}

		if (source->trackType() == UIValueTrack::TrackType::Animation) {
			(*instance)->m_animationTrack = static_cast<UIVector4AnimationTrack*>(source);
		}
		else if (source->trackType() == UIValueTrack::TrackType::Transition) {
			(*instance)->m_transitionTrack = static_cast<UIVector4TransitionTrack*>(source);
		}
		else {
			LN_UNREACHABLE();
		}

		(*instance)->m_sourceTrack = source;
		return true;
	}

	void reset(const Vector4& startValue)
	{
		m_startValue = startValue;
		m_time = 0.0f;
	}

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
	UIValueTrack* m_sourceTrack = nullptr;
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
	Result<> init();

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
    Result<> init();

private:
	void addCurve(detail::UIStyleAnimationElement element, float targetValue, float duration, EasingMode func, float delay);

	std::array<Curve, detail::UIStyleAnimationElement_Count_> m_curves;
};

} // namespace ln

