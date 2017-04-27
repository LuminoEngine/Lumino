#include <TestConfig.h>



namespace ln {
namespace a2 {

namespace detail {

class AnimationClock;

template<typename TValue>
class AnimationValueSetter
	: public RefObject
{
public:
	virtual ~AnimationValueSetter() = default;
	virtual void SetValue(const TValue& value) = 0;
};

template<typename TValue>
class AnimationVariableValueSetter
	: public AnimationValueSetter<TValue>
{
public:
	AnimationVariableValueSetter(TValue* ptr)
		: m_ptr(ptr)
	{}

	virtual ~AnimationVariableValueSetter() = default;

	virtual void SetValue(const TValue& value) override
	{
		*m_ptr = value;
	}

private:
	TValue*	m_ptr;
};




template<typename TValue>
TValue LerpAnimationValue(TValue v1, TValue v2, float t)
{
	return v1 + ((v2 - v1) * t);
}




class AnimationCurve	// TODO: AnimationTimeline (Event も)
	: public Object
{
public:
	AnimationCurve() = default;
	virtual ~AnimationCurve() = default;

public:

	/// 終端フレーム位置の取得
	virtual float GetLastTime() const = 0;

	//void SetWrapMode(WrapMode mode) { m_wrapMode = mode; }
	//WrapMode GetWrapMode() const { return m_wrapMode; }

private:
	//WrapMode	m_wrapMode;
};

template<typename TValue>
class ValueAnimationCurve
	: public AnimationCurve
{
public:
	ValueAnimationCurve() = default;
	virtual ~ValueAnimationCurve() = default;
	virtual TValue Interpolate(const TValue& startValue, float time) = 0;
};



template<typename TValue>
class EasingAnimationCurve
	: public ValueAnimationCurve<TValue>
{
public:
	static RefPtr<EasingAnimationCurve<TValue>> Create(const TValue& targetValue, double duration, EasingMode easingMode)
	{
		return RefPtr<EasingAnimationCurve<TValue>>::MakeRef(targetValue, duration, easingMode);
	}


	virtual float GetLastTime() const { return m_duration; }


	RefPtr<AnimationClock> Start(Object* targetObject, TValue* targetVariablePtr, const TValue& startValue);

public:
	EasingAnimationCurve(const TValue& targetValue, double duration, EasingMode easingMode)
		: m_targetValue(targetValue)
		, m_duration(duration)
		, m_easingFunction(AnimationUtilities::SelectEasingFunction<float, float>(easingMode))
	{
		//SetEasingMode(easingMode);
	}

	virtual ~EasingAnimationCurve() = default;

	virtual TValue Interpolate(const TValue& startValue, float time) override
	{
		float t = m_easingFunction(time, 0.0f, 1.0f, m_duration);
		return LerpAnimationValue(startValue, m_targetValue, t);
	}

private:
	TValue		m_targetValue;
	float		m_duration;
	std::function<TValue(float, float, float, float)>	m_easingFunction;
};


using FloatEasingAnimationCurve = EasingAnimationCurve<float>;
using Vector4EasingAnimationCurve = EasingAnimationCurve<Vector4>;



/**
	@brief		
*/
class AnimationClock
	: public Object
{
public:
	AnimationClock()
		: m_targetObject()
		, m_currentTime(0)
		, m_isFinished(false)
	{}
	virtual ~AnimationClock() = default;
	void Initialize() {}
	void AdvanceTime(float deltaTime)
	{
		m_currentTime += deltaTime;
		UpdateValue(m_currentTime);
	}
	//bool IsFinished() const { return m_isFinished; }

protected:
	virtual void UpdateValue(float time) = 0;

private:
	tr::WeakRefPtr<AnimatableObject>	m_targetObject;
	//List<RefPtr<AnimationCurveInstanceBase>>	m_instanceList;
	float					m_currentTime;
	bool					m_isFinished;
};


template<typename TValue>
class ValueAnimationClock
	: public AnimationClock
{
public:
	ValueAnimationClock(Object* targetObject, AnimationValueSetter<TValue>* valueSetter, ValueAnimationCurve<TValue>* timeline, const TValue& startValue)
		: m_startValue(startValue)
		, m_valueTimeline(timeline)
		, m_valueSetter(valueSetter)
	{
	}

	virtual ~ValueAnimationClock() = default;

protected:
	virtual void UpdateValue(float time) override
	{
		m_valueSetter->SetValue(m_valueTimeline->Interpolate(m_startValue, time));
	}

private:
	TValue									m_startValue;
	RefPtr<ValueAnimationCurve<TValue>>		m_valueTimeline;
	RefPtr<AnimationValueSetter<TValue>>	m_valueSetter;

};






template<typename TValue>
RefPtr<AnimationClock> EasingAnimationCurve<TValue>::Start(Object* targetObject, TValue* targetVariablePtr, const TValue& startValue)
{
	auto setter = RefPtr<AnimationVariableValueSetter<TValue>>::MakeRef(targetVariablePtr);
	auto ptr = RefPtr<ValueAnimationClock<TValue>>::MakeRef(targetObject, setter, this, startValue);
	return ptr;
}



}
}
}





class Test_Animation : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Animation, Basic)
{
	float value = 0;
	auto curve = ln::a2::detail::FloatEasingAnimationCurve::Create(5.0f, 2.0f, EasingMode::Linear);	// リソース。スタイルとかの。
	auto clock = curve->Start(nullptr, &value, 0.0f);

	clock->AdvanceTime(1.0f);



}

