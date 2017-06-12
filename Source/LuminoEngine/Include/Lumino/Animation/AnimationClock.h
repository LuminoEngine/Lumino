
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Animation/AnimationTimeline.h>

LN_NAMESPACE_BEGIN
class AnimationClock;
namespace detail { class AnimationTimelineInstance; }


/**
	@brief
*/
class AnimationClock
	: public Object
{
public:
	//template<typename TValue>
	//static RefPtr<AnimationClock> Create(const TValue& startValue, const Delegate<void(void)>& setCallback, const Delegate<void(void)>& endCallback);

	static RefPtr<AnimationClock> Create();

	AnimationClock();
	AnimationClock(AnimationTimeline* timeline, const Delegate<void(void)>& endCallback);
	virtual ~AnimationClock() = default;
	void initialize() {}
	void AdvanceTime(float deltaTime);
	bool IsFinished() const;

	//template<typename TValue>
	//void Start(AbstractValueAnimationTimeline* timeline, const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
	//{
	//	m_timelineInstance = detail::CreateTimelineInstance(static_cast<ValueAnimationTimeline<TValue>*>(timeline), startValue, setCallback, endCallback);
	//	//m_currentTime = 0;
	//	AddManager();
	//}

	void Start(float startValue, float targetValue, float duration, EasingMode easingMode, const Delegate<void(float)>& setCallback, const Delegate<void(void)>& endCallback);

	// TODO: private
	void AddManager();

//protected:
//	virtual void UpdateValue(float time) = 0;

private:
	//AnimationTimeline*			m_timeline;
	RefPtr<detail::AnimationTimelineInstance>	m_timelineInstance;
	//Delegate<void(void)>	m_endCallback;
};





namespace detail {


//==============================================================================
// Decide how to set target the calculated value.
template<typename TValue>
class AnimationValueSetter
	: public RefObject
{
public:
	virtual ~AnimationValueSetter() = default;
	virtual void SetValue(const TValue& value) = 0;
};

// to pointer
template<typename TValue>
class AnimationVariableValueSetter
	: public AnimationValueSetter<TValue>
{
public:
	AnimationVariableValueSetter(TValue* ptr) : m_ptr(ptr) {}
	virtual ~AnimationVariableValueSetter() = default;
	virtual void SetValue(const TValue& value) override { *m_ptr = value; }

private:
	TValue*	m_ptr;
};

// to callback
template<typename TValue, typename TDelegate>
class AnimationDelegateValueSetter
	: public AnimationValueSetter<TValue>
{
public:
	AnimationDelegateValueSetter(const TDelegate& callback) : m_callback(callback) {}
	virtual ~AnimationDelegateValueSetter() = default;
	virtual void SetValue(const TValue& value) override { m_callback(value); }

private:
	TDelegate	m_callback;
};




//==============================================================================
class AnimationTimelineInstance : public RefObject
{
public:
	void AdvanceTime(float deltaTime);
	bool IsFinished() const;
	virtual void UpdateTime(float previousTime, float currentTime) = 0;

protected:
	AnimationTimelineInstance(AnimationTimeline* timeline, const Delegate<void(void)>& endCallback)
		: m_timeline(timeline)
		, m_endCallback(endCallback)
		, m_targetObject()
		, m_currentTime(0)
	{}

private:
	RefPtr<AnimationTimeline>	m_timeline;
	Delegate<void(void)>		m_endCallback;
	tr::WeakRefPtr<Object>		m_targetObject;
	float						m_currentTime;
};

template<typename TValue>
class ValueAnimationTimelineInstance
	: public AnimationTimelineInstance
{
public:
	ValueAnimationTimelineInstance(Object* targetObject, AnimationValueSetter<TValue>* valueSetter, ValueAnimationTimeline<TValue>* timeline, const TValue& startValue, const Delegate<void(void)>& endCallback)
		: AnimationTimelineInstance(timeline, endCallback)
		, m_startValue(startValue)
		, m_valueTimeline(timeline)
		, m_valueSetter(valueSetter)
	{
	}

	virtual ~ValueAnimationTimelineInstance() = default;

protected:
	virtual void UpdateTime(float previousTime, float currentTime) override
	{
		m_valueSetter->SetValue(m_valueTimeline->Interpolate(m_startValue, Math::Clamp(currentTime, 0, m_valueTimeline->GetLastTime())));
	}

private:
	TValue									m_startValue;
	RefPtr<ValueAnimationTimeline<TValue>>	m_valueTimeline;
	RefPtr<AnimationValueSetter<TValue>>	m_valueSetter;
};

//==============================================================================
template<typename TValue>
RefPtr<ValueAnimationTimelineInstance<TValue>> CreateTimelineInstance(ValueAnimationTimeline<TValue>* timeline, const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
{
	auto setter = RefPtr<AnimationDelegateValueSetter<TValue, Delegate<void(TValue)>>>::MakeRef(setCallback);
	auto instance = RefPtr<ValueAnimationTimelineInstance<TValue>>::MakeRef(nullptr, setter, timeline, startValue, endCallback);
	return instance;
}

} // namespace detail


//
//template<typename TValue>
//RefPtr<AnimationClock> EasingAnimationTimeline<TValue>::Start(Object* targetObject, TValue* targetVariablePtr, const TValue& startValue, const Delegate<void(void)>& endCallback)
//{
//	auto setter = RefPtr<detail::AnimationVariableValueSetter<TValue>>::MakeRef(targetVariablePtr);
//	auto clock = RefPtr<detail::ValueAnimationClock<TValue>>::MakeRef(targetObject, setter, this, startValue, endCallback);
//	clock->AddManager();
//	return clock;
//}

//template<typename TValue>
//static RefPtr<AnimationClock> AnimationClock::Create(const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
//{
//	auto setter = RefPtr<detail::AnimationDelegateValueSetter<TValue>>::MakeRef(setCallback);
//	auto clock = RefPtr<detail::ValueAnimationClock<TValue>>::MakeRef(nullptr, setter, this, startValue, endCallback);
//	return clock;
//}

LN_NAMESPACE_END
