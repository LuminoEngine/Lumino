
#include "Internal.h"
#include <Lumino/Animation/AnimationClock.h>
#include <Lumino/Animation/AnimationCurve.h>

LN_NAMESPACE_BEGIN

namespace detail
{

	void RefrectionObjectAnimationData::OnPropertyChangedByLocal(Object* owner, const tr::Property* prop)
	{
		AnimationClock::DeactivatePropertyAnimation(owner, prop);
	}

} // namespace detail



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClock::AnimationClock()
	: m_currentTime(0)
	, m_isFinished(false)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClock::~AnimationClock()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::Initialize(Object* targetObject)
{
	m_targetObject = targetObject;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void AnimationClock::Initialize(AnimationClockArgs* list, int listCount)
//{
//	for (int iArgs = 0; iArgs < listCount; ++iArgs)
//	{
//		const AnimationClockArgs& timeline = list[iArgs];
//
//		// タイムラインの適用先を選択する
//		Object* target = timeline.targetObject;
//		//if (timeline->GetTargetName().IsEmpty()) {
//		//	target = owner;
//		//}
//		//else {
//		//	target = VisualTreeHelper::FindChildByName(owner, timeline->GetTargetName());
//		//}
//
//		tr::Property* prop = timeline.targetProperty;
//
//		// タイムラインの適用先プロパティを検索する
//		if (target != nullptr && prop != nullptr)
//		{
//			TimeLineInstance tli;
//			tli.OwnerTimeLine = timeline;
//			tli.TargetElement = target;
//			tli.TargetProperty = prop;
//			tli.StartValue = target->GetPropertyValue(prop);	// 現在のプロパティ値を開始値とする
//			tli.Active = true;
//			m_timeLineInstanceList.Add(tli);
//
//			// 再生中のアニメの中に同じターゲットの同じプロパティをアニメーションしているものがあれば停止する
//			auto& playingList = owner->GetAnimationClockList();
//			for (auto& clock : playingList)
//			{
//				for (auto& playingTimeLineInst : clock->m_timeLineInstanceList)
//				{
//					if (playingTimeLineInst.OwnerTimeLine->GetTargetName() == timeline->GetTargetName() &&
//						playingTimeLineInst.OwnerTimeLine->GetTargetProperty() == timeline->GetTargetProperty())
//					{
//						playingTimeLineInst.Active = false;
//					}
//				}
//			}
//		}
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::SetTime(double time)
{
	// とりあえず true にしておいて、タイムラインが1つでも実行中だったら false にする
	m_isFinished = true;

	for (RefPtr<AnimationCurveInstance>& tli : m_instanceList)
	{
		if (tli->isActive)
		{
			bool r = tli->owner->ApplyPropertyAnimation(tli, time);
			if (r) {
				m_isFinished = false;
			}
		}
	}

	// 終了したら Manager の時間管理から外す…のだが、
	// この関数の中ではまだイテレート中である。m_isFinished フラグを Manager で見て、そちらで外してもらう。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::AdvanceTime(float deltaTime)
{
	m_currentTime += deltaTime;
	SetTime(m_currentTime);
}


LN_NAMESPACE_END
