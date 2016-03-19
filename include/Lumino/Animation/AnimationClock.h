
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include "../../src/Animation/AnimationCurve.h"

LN_NAMESPACE_BEGIN

struct AnimationClockArgs
{
	Animation::AnimationCurve* curve;
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
	//void Initialize(AnimationClockArgs* list, int listCount);

	void SetTime(double time);
	void AdvanceTime(float deltaTime);

	template<typename TCurve, typename TValue>
	void AddAnimationCurve(TCurve* curve, Object* targetObject, const tr::Property* targetProperty, const TValue& startValue)
	{
		Animation::AnimationCurveInstance* inst = curve->CreateAnimationCurveInstance(targetObject, targetProperty, startValue);
		m_instanceList.Add(inst);
		// TODO:再生中のアニメの中に同じターゲットの同じプロパティをアニメーションしているものがあれば停止する
	}

private:
	Array<RefPtr<Animation::AnimationCurveInstance>>	m_instanceList;
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
