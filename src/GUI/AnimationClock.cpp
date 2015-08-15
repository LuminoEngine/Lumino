
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/AnimationClock.h>
#include <Lumino/GUI/VisualTreeHelper.h>

namespace Lumino
{
namespace GUI
{

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
AnimationClock::AnimationClock(Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines)
	: m_sourceStoryboard(sourceStoryboard)
	, m_timeLineInstanceList()
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
				tli.StartValue = target->GetPropertyValue(prop);
				m_timeLineInstanceList.Add(tli);
			}
		}
	}

	// 初期値セットの意味も込めて、ここでまずは 0 タイムで値をセットしておく。
	SetTime(0);
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
	for (TimeLineInstance& tli : m_timeLineInstanceList)
	{
		tli.OwnerTimeLine->Apply(tli.TargetElement, tli.TargetProperty, tli.StartValue, time);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::StopInternal()
{
	// TODO: Manager から外す
}

} // namespace GUI
} // namespace Lumino

