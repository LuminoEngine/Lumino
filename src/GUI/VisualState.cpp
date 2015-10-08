/*
	・StoryBoard とアニメーションの仕組みについて

		Storyboard は複数の AnimationTimeline を持ち、再生/停止を行う。

		AnimationTimeline(派生 FloatEasing 等) はどの名前の要素の
		どのプロパティにどんなアニメーションを適用したいかを管理する。

		Storyboard::Begin ではその Storyboard が持っているすべての
		AnimationTimeline をまとめた1つの AnimationClock を生成する。

		AnimationClock のコンストラクタでは、AnimationTimeline の設定先となる
		要素を検索する。ここで AnimationTimeline が持っている TargetName が使われる。

		生成された AnimationClock は Context に登録され、
		時間の経過に合わせて AdvanceTime() が呼ばれる。

		AnimationClock::AdvanceTime() で、ターゲットのプロパティに実際に値を設定する。

*/
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/VisualState.h>
#include <Lumino/GUI/VisualTreeHelper.h>
#include "GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// FloatEasing
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing* FloatEasing::Create(
	const String& targetName,
	const String& targetProperty,
	float targetValue,
	float duration,
	Animation::EasingMode easingMode)
{
	GCPtr<FloatEasing> easing = LN_NEW FloatEasing();
	easing->SetTargetName(targetName);
	easing->SetTargetProperty(targetProperty);
	easing->SetTargetValue(targetValue);
	easing->SetDuration(duration);
	easing->SetEasingMode(easingMode);
	easing.SafeAddRef();
	return easing;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing::FloatEasing()
	:/* m_targetName()
	, m_targetProperty(NULL)
	, */m_targetValue(0)
	, m_easingMode(Animation::EasingMode::Linear)
	//, m_duration(0)
	, m_easingFunction()
{
	SetEasingMode(Animation::EasingMode::Linear);
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
void FloatEasing::SetEasingMode(Animation::EasingMode easingMode)
{
	m_easingMode = easingMode;
	m_easingFunction = AnimationUtilities::SelectEasingFunction<float>(m_easingMode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FloatEasing::Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time)
{
	float value = 0;

	// 経過時間 0 の場合はそのままセットで良い。0除算対策の意味も込めて。
	// また、時間が既に終端を超えていたり、比較関数が無い場合も直値セット。
	if (m_duration == 0 || m_duration <= time || m_easingFunction == nullptr)
	{
		value = m_targetValue;
	}
	// 時間が 0 以前の場合は初期値。
	else if (time <= 0)
	{
		value = Variant::Cast<float>(startValue);
	}
	// 補間で求める
	else
	{
		float b = Variant::Cast<float>(startValue);
		value = m_easingFunction(time, b, m_targetValue - b, m_duration);
	}

	targetElement->SetTypedPropertyValue<float>(targetProp, value);
	return (time < m_duration);
}

//=============================================================================
// VisualState
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState(GUIManagerImpl* manager)
	: m_manager(manager)
{
	m_storyboard.Attach(LN_NEW Storyboard(m_manager));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState(GUIManagerImpl* manager, const String& name)
	: m_manager(manager)
	, m_name(name)
{
	m_storyboard.Attach(LN_NEW Storyboard(m_manager));
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

	// 先に stateName がカレントではないかを確認する。
	// 1つでも同じ名前のモノがあれば中断。(同名ステートの追加は許可しない)
	VisualStateGroupList* groupList = control->GetVisualStateGroups();
	for (auto* group : *groupList)
	{
		if (group->GetCurrentState() != NULL &&
			group->GetCurrentState()->GetName() == stateName)
		{
			return;
		}
	}

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
Storyboard::Storyboard(GUIManagerImpl* manager)
	: m_manager(manager)
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
	RefPtr<AnimationClock> clock(LN_NEW AnimationClock(target->GetContext(), this, target, &m_animationTimelineList));

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
const String VisualStatus::Pressed = _T("Pressed");
const String VisualStatus::Disabled = _T("Disabled");

LN_NAMESPACE_GUI_END
} // namespace Lumino

