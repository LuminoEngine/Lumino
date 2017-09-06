
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Animation/AnimationTimeline.h>
#include <Lumino/Animation/AnimationUtilities.h>

LN_NAMESPACE_BEGIN
class AnimationClock;


template<typename TValue>
TValue lerpAnimationValue(TValue v1, TValue v2, float t)
{
	return v1 + ((v2 - v1) * t);
}

/**
	@brief
*/
class AnimationTimeline
	: public Object
{
public:
	AnimationTimeline() = default;
	virtual ~AnimationTimeline() = default;

public:

	/// 終端フレーム位置の取得
	virtual float getLastTime() const = 0;

	//void setWrapMode(WrapMode mode) { m_wrapMode = mode; }
	//WrapMode getWrapMode() const { return m_wrapMode; }

private:
	//WrapMode	m_wrapMode;
};

/**
	@brief
*/
class AbstractValueAnimationTimeline
	: public AnimationTimeline
{
public:
	AbstractValueAnimationTimeline() = default;
	virtual ~AbstractValueAnimationTimeline() = default;
};



template<typename TValue>
class ValueAnimationTimeline
	: public AbstractValueAnimationTimeline
{
public:
	ValueAnimationTimeline() = default;
	virtual ~ValueAnimationTimeline() = default;
	virtual float getLastTime() const = 0;
	virtual TValue interpolate(const TValue& startValue, float time) = 0;
};



template<typename TValue>
class EasingAnimationTimeline
	: public ValueAnimationTimeline<TValue>
{
public:
	static Ref<EasingAnimationTimeline<TValue>> create(const TValue& targetValue, double duration, EasingMode easingMode)
	{
		return Ref<EasingAnimationTimeline<TValue>>::makeRef(targetValue, duration, easingMode);
	}


	virtual float getLastTime() const override { return m_duration; }

	Ref<AnimationClock> start(Object* targetObject, TValue* targetVariablePtr, const TValue& startValue, const Delegate<void(void)>& endCallback);

public:
	EasingAnimationTimeline(const TValue& targetValue, double duration, EasingMode easingMode)
		: m_targetValue(targetValue)
		, m_duration(duration)
		, m_easingFunction(AnimationUtilities::SelectEasingFunction<float, float>(easingMode))
	{
	}

	virtual ~EasingAnimationTimeline() = default;

	virtual TValue interpolate(const TValue& startValue, float time) override
	{
		float t = m_easingFunction(time, 0.0f, 1.0f, m_duration);
		return lerpAnimationValue(startValue, m_targetValue, t);
	}

private:
	TValue		m_targetValue;
	float		m_duration;
	std::function<TValue(float, float, float, float)>	m_easingFunction;
};


using FloatEasingAnimationTimeline = EasingAnimationTimeline<float>;
using Vector4EasingAnimationTimeline = EasingAnimationTimeline<Vector4>;

LN_NAMESPACE_END
