
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		
*/
class AnimationClock
	: public Object
{
public:
	AnimationClock(GUIContext* context, Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines);
	virtual ~AnimationClock();

	bool IsFinished() const { return m_isFinished; }
	
	// TODO: internal
	void SetTime(float time);
	void AdvanceTime(float deltaTime);

private:
	void StopInternal();

private:
	struct TimeLineInstance
	{
		RefPtr<AnimationTimeline>	OwnerTimeLine;
		UIElement*	TargetElement;		// TODO: アニメ実行中に Target がデストラクトされた時の対応
		Property*	TargetProperty;
		Variant		StartValue;
		bool		Active;			// true の場合、実際に再生する (古い再生を停止するときに false にする。本来はリストから delete しても良いのだが、メモリ効率的に。)
	};

	GUIContext*				m_context;
	Storyboard*				m_sourceStoryboard;		// 停止するときのキーにしたいので参照カウントを上げる必要はない
	Array<TimeLineInstance>	m_timeLineInstanceList;
	float					m_currentTime;
	bool					m_isFinished;

	friend class Storyboard;
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
