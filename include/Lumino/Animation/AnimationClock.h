
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Animation/AnimationCurve.h>
#include "AnimatableObject.h"


LN_NAMESPACE_BEGIN

struct AnimationClockArgs
{
	AnimationCurve* curve;
	Object*			targetObject;
	tr::Property*	targetProperty;
};

/**
	@brief		
*/
class AnimationClock
	: public Object
{
public:
	AnimationClock();
	virtual ~AnimationClock();
	// TODO: internal
	void Initialize(AnimatableObject* targetObject);
	const tr::WeakRefPtr<AnimatableObject>& GetTargetObject() const { return m_targetObject; }

	void SetTime(double time);
	void AdvanceTime(float deltaTime);
	bool IsFinished() const { return m_isFinished; }

	template<typename TCurve, typename TValue>
	void AddAnimationCurve(TCurve* curve, AnimatableObject* targetObject, const tr::Property* targetProperty, const TValue& startValue)
	{
		targetObject->DeactivatePropertyAnimation(targetProperty);

		RefPtr<AnimationCurveInstance> inst(curve->CreateAnimationCurveInstance(targetObject, targetProperty, startValue), false);
		m_instanceList.Add(inst);
	}


private:
	friend class AnimatableObject;

	tr::WeakRefPtr<AnimatableObject>	m_targetObject;
	Array<RefPtr<AnimationCurveInstance>>	m_instanceList;
	float					m_currentTime;
	bool					m_isFinished;

//	AnimationClock(GUIContext* context, Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines);
//	virtual ~AnimationClock();
//
//	bool IsFinished() const { return m_isFinished; }
//	
//	// TODO: internal
//	void SetTime(float time);
//	void AdvanceTime(float deltaTime);
//
//private:
//	void StopInternal();
//
//private:
//	struct TimeLineInstance
//	{
//		RefPtr<AnimationTimeline>	OwnerTimeLine;
//		UIElement*	TargetElement;		// TODO: アニメ実行中に Target がデストラクトされた時の対応
//		Property*	TargetProperty;
//		Variant		StartValue;
//		bool		Active;			// true の場合、実際に再生する (古い再生を停止するときに false にする。本来はリストから delete しても良いのだが、メモリ効率的に。)
//	};
//
//	GUIContext*				m_context;
//	Storyboard*				m_sourceStoryboard;		// 停止するときのキーにしたいので参照カウントを上げる必要はない
//	Array<TimeLineInstance>	m_timeLineInstanceList;
//	
//	
//
//	friend class Storyboard;
};


LN_NAMESPACE_END
