
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


namespace detail
{

	class RefrectionObjectAnimationData
		: public tr::detail::ReflectionObjectAnimationData
	{
	public:
		virtual ~RefrectionObjectAnimationData() {}
		Array<RefPtr<AnimationClock>>	playingAnimationClockList;
	};

	//static inline RefrectionObjectAnimationData* RequestAnimationData(Object* obj)
	//{
	//	auto* data = reinterpret_cast<RefrectionObjectAnimationData*>(tr::ReflectionHelper::GetAnimationData(obj));
	//	if (data == nullptr)
	//	{
	//		data = LN_NEW 
	//	}
	//}

} // namespace detail

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
	void Initialize(Object* targetObject);
	const tr::WeakRefPtr<Object>& GetTargetObject() const { return m_targetObject; }

	void SetTime(double time);
	void AdvanceTime(float deltaTime);
	bool IsFinished() const { return m_isFinished; }

	template<typename TCurve, typename TValue>
	void AddAnimationCurve(TCurve* curve, Object* targetObject, const tr::Property* targetProperty, const TValue& startValue)
	{
		// 再生中のアニメの中に同じターゲットの同じプロパティをアニメーションしているものがあれば停止する
		auto* data = tr::ReflectionHelper::RequestAnimationData<Object, detail::RefrectionObjectAnimationData>(targetObject);
		for (auto& clock : data->playingAnimationClockList)
		{
			for (auto& playingCurveLineInst : clock->m_instanceList)
			{
				if (playingCurveLineInst->targetObject == targetObject &&
					playingCurveLineInst->targetProperty == targetProperty)
				{
					playingCurveLineInst->isActive = false;
				}
			}
		}

		RefPtr<Animation::AnimationCurveInstance> inst(curve->CreateAnimationCurveInstance(targetObject, targetProperty, startValue), false);
		m_instanceList.Add(inst);

		
	}

private:
	tr::WeakRefPtr<Object>	m_targetObject;
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
