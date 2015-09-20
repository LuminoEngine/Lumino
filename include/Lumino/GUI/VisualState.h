
#pragma once
#include "Common.h"
#include "../Animation/AnimationUtilities.h"
#include "../Variant.h"
#include "AnimationClock.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		
	<FloatTimeline From="0" To="60" Duration="500"	時間はミリ秒
		TargetName="textBlock"
		TargetProperty="FontSize" />
*/
class FloatTimeline		// TODO: いらないかも
	: public CoreObject
{
public:
	FloatTimeline();
	virtual ~FloatTimeline();

protected:
	//Animation::FloatAnimationCurve	m_curve;
};

class FloatEasing
	: public AnimationTimeline
{
public:
	FloatEasing();
	virtual ~FloatEasing();

	//void SetTargetName(const String& name) { m_targetName = name; }
	//void SetTargetProperty(const Property* prop) { m_targetProperty = prop; }
	void SetTargetValue(float value) { m_targetValue = value; }
	void SetEasingMode(Animation::EasingMode easingMode);
	void SetDuration(float duration) { m_duration = duration; }

protected:
	virtual bool Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time);

private:
	//String					m_targetName;
	//const Property*			m_targetProperty;
	float					m_targetValue;
	Animation::EasingMode	m_easingMode;
	//float					m_duration;
	AnimationUtilities::FloatEasingFunction	m_easingFunction;

};

/**
	@brief		
*/
class Storyboard
	: public CoreObject
{
public:
	Storyboard(GUIManagerImpl* manager);
	virtual ~Storyboard();

	void AddTimeline(AnimationTimeline* timeline);

	//Array< RefPtr<AnimationTimeline*> > GetChildren();
	
	/**
		@brief	指定した要素にアニメーションを適用して開始します。
	*/
	void Begin(UIElement* target);

	/**
		@brief	指定した要素に割り当てられている、この Storyboard 用に作成された AnimationClock を停止します。
	*/
	void Stop(UIElement* target);

private:
	GUIManagerImpl*	m_manager;
	Array< RefPtr<AnimationTimeline> >	m_animationTimelineList;
};

/**
	@brief		
*/
class VisualState
	: public CoreObject
{
	//[RuntimeNamePropertyAttribute("Name")]
	//[ContentPropertyAttribute("Storyboard")]
public:
	VisualState(GUIManagerImpl* manager);
	VisualState(GUIManagerImpl* manager, const String& name);
	virtual ~VisualState();

	const String& GetName() const { return m_name; }
	Storyboard* GetStoryboard() const { return m_storyboard; }

protected:
	GUIManagerImpl* m_manager;
	String	m_name;
	RefPtr<Storyboard>	m_storyboard;
};

typedef GenericCoreList<VisualState*>	VisualStateList;
	
/**
	@brief		
	@details	VisualStateGroup には、同時に使用できない状態を含めます。 たとえば、CheckBox には 2 つの VisualStateGroup オブジェクトがあります。
				一方のオブジェクトには、Normal、MouseOver、Pressed、および Disabled の状態が格納されます。 
				もう一方のオブジェクトには、Checked、UnChecked、および Indeterminate の状態が格納されます。 
				CheckBox は同時に MouseOver の状態と UnChecked の状態になることができますが、
				同時に MouseOver の状態と Pressed の状態になることはできません。
				https://msdn.microsoft.com/ja-jp/library/system.windows.visualstategroup%28v=vs.110%29.aspx
*/
class VisualStateGroup
	: public CoreObject
{
public:
	VisualStateGroup();
	VisualStateGroup(const String& groupName);
	virtual ~VisualStateGroup();

	/// このグループ内で現在アクティブであり、コントロールに適用されている VisualState を取得します。
	/// (停止するときに使用する)
	VisualState* GetCurrentState() const { return m_currentState; }

	VisualStateList* GetVisualStateList() { return &m_status; }

	void AddState(VisualState* state) { m_status.Add(state); }

private:
	void SetCurrentState(VisualState* state) { m_currentState = state; }

private:
	String			m_name;
	VisualStateList	m_status;	// TODO: * に。
	VisualState*	m_currentState;
	friend class VisualStateManager;
};

typedef GenericCoreList<VisualStateGroup*>	VisualStateGroupList;
	
/**
	@brief		
	@note		・VisualStateGroups は複数の UIElement から共有される。→ というより、Style が共有される。
				・WPF の VisualStateManager.GotoState() は static メソッド。
*/
class VisualStateManager
	: public CoreObject
{
public:
	VisualStateManager();
	virtual ~VisualStateManager();
	
	/// UI要素の状態を切り替えます。
	static void GoToState(Control* control, const String& stateName);

protected:

};



class VisualStatus
{
public:
	static const String	Normal;		/* "Normal" 状態の名前です。*/
	static const String	MouseOver;	/* "MouseOver" 状態の名前です。*/
	static const String	Pressed;	/* "Pressed" 状態の名前です。*/
	static const String	Disabled;	/* "Disabled" 状態の名前です。*/

};
	

LN_NAMESPACE_GUI_END
} // namespace Lumino
