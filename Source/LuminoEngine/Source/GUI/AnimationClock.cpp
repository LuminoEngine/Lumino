
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/AnimationClock.h>
#include <Lumino/GUI/VisualTreeHelper.h>
#include <Lumino/GUI/GUIContext.h>
#include "UIManagerImpl.h"
#include "GUIHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// AnimationTimeline
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTimeline::AnimationTimeline()
	: m_targetName()
	, m_targetProperty()
	, m_duration(0)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTimeline::~AnimationTimeline()
{

}


//=============================================================================
// AnimationClock
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClock::AnimationClock(GUIContext* context, Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines)
	: m_context(context)
	, m_sourceStoryboard(sourceStoryboard)
	, m_timeLineInstanceList()
	, m_currentTime(0)
	, m_isFinished(false)
{
	for (AnimationTimeline* timeline : *timelines)
	{
		// タイムラインの適用先を選択する
		UIElement* target;
		if (timeline->GetTargetName().IsEmpty()) {
			target = owner;
		}
		else {
			target = VisualTreeHelper::FindChildByName(owner, timeline->GetTargetName());
		}

		// タイムラインの適用先プロパティを検索する
		if (target != NULL)
		{
			// TODO:TypeInfoのstatic関数にしたい
			Property* prop = GetTypeInfo(target)->FindProperty(timeline->GetTargetProperty());
			if (prop != NULL)
			{
				TimeLineInstance tli;
				tli.OwnerTimeLine = timeline;
				tli.TargetElement = target;
				tli.TargetProperty = prop;
				tli.StartValue = target->GetPropertyValue(prop);	// 現在のプロパティ値を開始値とする
				tli.Active = true;
				m_timeLineInstanceList.Add(tli);

				// 再生中のアニメの中に同じターゲットの同じプロパティをアニメーションしているものがあれば停止する
				auto& playingList = owner->GetAnimationClockList();
				for (auto& clock : playingList)
				{
					for (auto& playingTimeLineInst : clock->m_timeLineInstanceList)
					{
						if (playingTimeLineInst.OwnerTimeLine->GetTargetName() == timeline->GetTargetName() &&
							playingTimeLineInst.OwnerTimeLine->GetTargetProperty() == timeline->GetTargetProperty())
						{
							playingTimeLineInst.Active = false;
						}
					}
				}
			}
		}
	}

	// 初期値セットの意味も込めて、ここでまずは 0 タイムで値をセットしておく。
	SetTime(0);

	GUIHelper::AddAnimationClock(m_context, this);
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
void AnimationClock::SetTime(float time)
{
	// とりあえず true にしておいて、タイムラインが1つでも実行中だったら false にする
	m_isFinished = true;

	for (TimeLineInstance& tli : m_timeLineInstanceList)
	{
		if (tli.Active)
		{
			bool r = tli.OwnerTimeLine->Apply(tli.TargetElement, tli.TargetProperty, tli.StartValue, time);
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
void AnimationClock::AdvanceTime(float elapsedTime)
{
	m_currentTime += elapsedTime;
	SetTime(m_currentTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::StopInternal()
{
	// Manager から外す
	GUIHelper::RemoveAnimationClock(m_context, this);
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

