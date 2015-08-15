
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/VisualState.h>
#include <Lumino/GUI/VisualTreeHelper.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// FloatEasing
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing::FloatEasing()
	:/* m_targetName()
	, m_targetProperty(NULL)
	, */m_targetValue(0)
	, m_easingMode(Animation::EasingMode::Linear)
	//, m_duration(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing::~FloatEasing()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FloatEasing::Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time)
{
	if (m_duration == 0)
	{
		// 時間 0 の場合はそのままセットで良い。0除算対策の意味も込めて。
		targetElement->SetTypedPropertyValue<float>(targetProp, m_targetValue);
	}
	else
	{
		LN_THROW(0, NotImplementedException);
	}
}

//=============================================================================
// VisualState
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState(const String& name)
	: m_name(name)
{
	m_storyboard.Attach(LN_NEW Storyboard());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::~VisualState()
{
}

//=============================================================================
// VisualStateGroup
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::VisualStateGroup()
	: m_name()
	, m_currentState(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::VisualStateGroup(const String& name)
	: m_name(name)
	, m_currentState(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::~VisualStateGroup()
{
}

//=============================================================================
// VisualStateGroup
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateManager::VisualStateManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateManager::~VisualStateManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualStateManager::GoToState(Control* control, const String& stateName)
{
	LN_CHECK_ARGS_RETURN(control != NULL);
	if (stateName.IsEmpty()) { return; }

	VisualStateGroupList* groupList = control->GetVisualStateGroups();
	for (auto* group : *groupList)
	{
		VisualStateList* stateList = group->GetVisualStateList();
		VisualStateList::iterator itr = std::find_if(stateList->begin(), stateList->end(), [stateName](VisualState* state) { return state->GetName() == stateName; });
		if (itr != stateList->end())
		{
			// 再生中のアニメを止める
			VisualState* current = group->GetCurrentState();
			if (current != NULL) {
				current->GetStoryboard()->Stop(control);
			}

			group->SetCurrentState(*itr);				// カレント変更
			(*itr)->GetStoryboard()->Begin(control);	// アニメーション開始
			return;	// 処理終了
		}
	}
}


//=============================================================================
// Storyboard
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Storyboard::Storyboard()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Storyboard::~Storyboard()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::AddTimeline(AnimationTimeline* timeline)
{
	RefPtr<AnimationTimeline> t(timeline, true);
	m_animationTimelineList.Add(t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::Begin(UIElement* target)
{
	// TODO: AnimationClock は頻繁に作成されるのでキャッシュしたい
	RefPtr<AnimationClock> clock(LN_NEW AnimationClock(this, target, &m_animationTimelineList));

	// TODO: Manager にも登録してアニメしてもらう
	GUIHelper::UIElement_GetAnimationClockList(target)->Add(clock);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::Stop(UIElement* target)
{
	Array< RefPtr<AnimationClock> >* list = GUIHelper::UIElement_GetAnimationClockList(target);
	Array< RefPtr<AnimationClock> >::iterator itr = list->begin();
	for (; itr != list->end();)
	{
		if ((*itr)->m_sourceStoryboard == this)
		{
			(*itr)->StopInternal();
			itr = list->erase(itr);
		}
		else {
			++itr;
		}
	}
}


//=============================================================================
// VisualStatus
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String VisualStatus::Normal = _T("Normal");
const String VisualStatus::MouseOver = _T("MouseOver");

} // namespace GUI
} // namespace Lumino

