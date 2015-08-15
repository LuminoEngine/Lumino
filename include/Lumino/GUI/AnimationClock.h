
#pragma once
#include "Common.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief		
	@note		
*/
class AnimationTimeline
	: public CoreObject
{
public:
	AnimationTimeline();
	virtual ~AnimationTimeline();

	/// アニメーション化するオブジェクトの名前 (x;key で指定された名前)
	/// 指定していない場合は論理要素がターゲットとなる。
	void SetTargetName(const String& name) { m_targetName = name; }

	const String& GetTargetName() const { return m_targetName; }

	/// アニメーション化するプロパティの名前
	void SetTargetProperty(const String& name) { m_targetProperty = name; }	// WPF では添付プロパティで型は PropertyPath

	const String& GetTargetProperty() const { return m_targetProperty; }

	void SetDuration(float duration) { m_duration = duration; }

protected:
	virtual void Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time) = 0;

protected:
	String	m_targetName;		///< ターゲットの UI 要素名。ビジュアルツリーから対象要素を検索するときに使用する。
	String	m_targetProperty;	///< ターゲットプロパティ名
	double	m_duration;			///< 再生時間 (ミリ秒)

	friend class AnimationClock;
};
	

/**
	@brief		
	@note		このインスタンスは UIElement が保持する。
*/
class AnimationClock
	: public CoreObject
{
public:
	AnimationClock(Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines);
	virtual ~AnimationClock();
	
	// TODO: internal
	void SetTime(float time);
	void AdvanceTime(float elapsedTime);

private:
	void StopInternal();

private:
	struct TimeLineInstance
	{
		RefPtr<AnimationTimeline>	OwnerTimeLine;
		UIElement*	TargetElement;		// TODO: アニメ実行中に Target がデストラクトされた時の対応
		Property*	TargetProperty;
		Variant		StartValue;
	};

	Storyboard*	m_sourceStoryboard;		// 停止するときのキーにしたいので参照カウントを上げる必要はない
	Array<TimeLineInstance>	m_timeLineInstanceList;

	friend class Storyboard;
};
	
} // namespace GUI
} // namespace Lumino
