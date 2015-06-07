
#pragma once
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class VisualState
	: public CoreObject
{
	//[RuntimeNamePropertyAttribute("Name")]
	//[ContentPropertyAttribute("Storyboard")]
public:
	VisualState();
	VisualState(const String& name);
	virtual ~VisualState();

protected:
	String	m_name;
	Storyboard*	m_storyboard;
};
	
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
	VisualStateGroup(const String& name);
	virtual ~VisualStateGroup();
	
	/// このグループ内で現在アクティブであり、コントロールに適用されている VisualState を取得します。
	VisualState* GetCurrentState();

private:
	String	m_name;
};
	
/**
	@brief		
	@note		・VisualStateManager は複数の UIElement から共有される。
				・WPF の VisualStateManager.GotoState() は static メソッド。
*/
class VisualStateManager
	: public CoreObject
{
public:
	VisualStateManager();
	virtual ~VisualStateManager();
	
	/// UI要素の状態を切り替えます。
	void GoToState(UIElement* element, const String& stateName);

protected:

};


/**
	@brief		
	@note		1つのターゲットプロパティに1つの AnimationTimeline を適用する。
*/
class AnimationClock
	: public CoreObject
{
public:
	AnimationClock(UIElement* targetElement, const String& targetPropertyName);
	virtual ~AnimationClock();
	
	void AdvanceTime(double elapsedTime);

protected:
};

/**
	@brief		
	@note		
*/
class AnimationTimeline
	: public CoreObject
{
public:
	AnimationTimeline();
	AnimationTimeline(const String& targetName);
	virtual ~AnimationTimeline();

	/// アニメーション化するオブジェクトの名前 (x;key で指定された名前)
	void SetTargetName(const String& name) { m_targetName = name; }

	/// アニメーション化するプロパティの名前
	void SetTargetProperty(const String& name) { m_targetProperty = name; }	// WPF では添付プロパティで型は PropertyPath

protected:
	double	m_duration;			///< 再生時間 (ミリ秒)
	String	m_targetName;		///< ターゲットの UI 要素名。ビジュアルツリーから対象要素を検索するときに使用する。
	String	m_targetProperty;	///< ターゲットプロパティ名
};
	
/**
	@brief		
	<FloatTimeline From="0" To="60" Duration="500"	時間はミリ秒
		TargetName="textBlock"
		TargetProperty="FontSize" />
*/
class FloatTimeline
	: public CoreObject
{
public:
	FloatTimeline();
	virtual ~FloatTimeline();

protected:
	//Animation::FloatAnimationCurve	m_curve;
};

/**
	@brief		
*/
class Storyboard
	: public CoreObject
{
public:
	Storyboard();
	virtual ~Storyboard();

	void AddTimeline(AnimationTimeline* timeline);

	//ArrayList< RefPtr<AnimationTimeline*> > GetChildren();
	
	void Begin(UIElement* target);

private:
	ArrayList< RefPtr<AnimationTimeline> >	m_animationTimelineList;
};


	

} // namespace GUI
} // namespace Lumino
