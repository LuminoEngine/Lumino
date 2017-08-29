
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
	//static Ref<AnimationClock> create(const TValue& startValue, const Delegate<void(void)>& setCallback, const Delegate<void(void)>& endCallback);

	static Ref<AnimationClock> create();

	AnimationClock();
	AnimationClock(AnimationTimeline* timeline, const Delegate<void(void)>& endCallback);
	virtual ~AnimationClock() = default;
	bool initialize() { return Object::initialize(); }
	void advanceTime(float deltaTime);
	bool isFinished() const;

	//template<typename TValue>
	//void Start(AbstractValueAnimationTimeline* timeline, const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
	//{
	//	m_timelineInstance = detail::createTimelineInstance(static_cast<ValueAnimationTimeline<TValue>*>(timeline), startValue, setCallback, endCallback);
	//	//m_currentTime = 0;
	//	addManager();
	//}

	void start(float startValue, float targetValue, float duration, EasingMode easingMode, const Delegate<void(float)>& setCallback, const Delegate<void(void)>& endCallback);

	// TODO: private
	void addManager();

//protected:
//	virtual void updateValue(float time) = 0;

private:
	//AnimationTimeline*			m_timeline;
	Ref<detail::AnimationTimelineInstance>	m_timelineInstance;
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
	virtual void setValue(const TValue& value) = 0;
};

// to pointer
template<typename TValue>
class AnimationVariableValueSetter
	: public AnimationValueSetter<TValue>
{
public:
	AnimationVariableValueSetter(TValue* ptr) : m_ptr(ptr) {}
	virtual ~AnimationVariableValueSetter() = default;
	virtual void setValue(const TValue& value) override { *m_ptr = value; }

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
	virtual void setValue(const TValue& value) override { m_callback(value); }

private:
	TDelegate	m_callback;
};




//==============================================================================
class AnimationTimelineInstance : public RefObject
{
public:
	void advanceTime(float deltaTime);
	bool isFinished() const;
	virtual void updateTime(float previousTime, float currentTime) = 0;

protected:
	AnimationTimelineInstance(AnimationTimeline* timeline, const Delegate<void(void)>& endCallback)
		: m_timeline(timeline)
		, m_endCallback(endCallback)
		, m_targetObject()
		, m_currentTime(0)
	{}

private:
	Ref<AnimationTimeline>	m_timeline;
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
	virtual void updateTime(float previousTime, float currentTime) override
	{
		m_valueSetter->setValue(m_valueTimeline->interpolate(m_startValue, Math::clamp(currentTime, 0, m_valueTimeline->getLastTime())));
	}

private:
	TValue									m_startValue;
	Ref<ValueAnimationTimeline<TValue>>	m_valueTimeline;
	Ref<AnimationValueSetter<TValue>>	m_valueSetter;
};

//==============================================================================
template<typename TValue>
Ref<ValueAnimationTimelineInstance<TValue>> createTimelineInstance(ValueAnimationTimeline<TValue>* timeline, const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
{
	auto setter = Ref<AnimationDelegateValueSetter<TValue, Delegate<void(TValue)>>>::makeRef(setCallback);
	auto instance = Ref<ValueAnimationTimelineInstance<TValue>>::makeRef(nullptr, setter, timeline, startValue, endCallback);
	return instance;
}

} // namespace detail


//
//template<typename TValue>
//Ref<AnimationClock> EasingAnimationTimeline<TValue>::Start(Object* targetObject, TValue* targetVariablePtr, const TValue& startValue, const Delegate<void(void)>& endCallback)
//{
//	auto setter = Ref<detail::AnimationVariableValueSetter<TValue>>::MakeRef(targetVariablePtr);
//	auto clock = Ref<detail::ValueAnimationClock<TValue>>::MakeRef(targetObject, setter, this, startValue, endCallback);
//	clock->addManager();
//	return clock;
//}

//template<typename TValue>
//static Ref<AnimationClock> AnimationClock::create(const TValue& startValue, const Delegate<void(TValue)>& setCallback, const Delegate<void(void)>& endCallback)
//{
//	auto setter = Ref<detail::AnimationDelegateValueSetter<TValue>>::MakeRef(setCallback);
//	auto clock = Ref<detail::ValueAnimationClock<TValue>>::MakeRef(nullptr, setter, this, startValue, endCallback);
//	return clock;
//}

LN_NAMESPACE_END
