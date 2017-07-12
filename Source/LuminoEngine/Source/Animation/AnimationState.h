
#pragma once
#include <Lumino/Animation/Common.h>
#include <Lumino/Animation/EasingValue.h>
#include <Lumino/Animation/AnimationCurve.h>
#include "Detail.h"

LN_NAMESPACE_BEGIN

/// データモデルである AnimationClip に対するインスタンス
class AnimationState
	: public RefObject
{
public:
	AnimationState(AnimationClip* clip);
	~AnimationState();

public:
	/// 名前の取得
	const String& getName() const;

	/// AnimationTarget の再割り当てを行う
	void refresh(Animator* animator);

	/// 再生状態の設定
	void setPlayState(PlayState state);

	/// 再生状態の取得
	PlayState setPlayState() const { return m_state; }

	/// この AnimationState の同レイヤー内のブレンド率
	//void SetAddingBlendWeight(float weight) { m_addingBlendWeight = weight; }

	/// 時間を進める
	void advanceTime(double elapsedTime);

LN_INTERNAL_ACCESS:
	void fadeInLinerInternal(float duration);
	void fadeOutLinerInternal(float duration);

private:
	void clearTargetList();
	void setLocalTime(double time);

private:
	struct AnimationTarget
	{
		AnimationCurve*							Curve;		// このアニメーションは
		detail::AnimationTargetAttributeEntity*	Target;		// TargetState.Value に値をセットする
	};

	enum class InternalFade
	{
		None,
		FadeIn,
		FadeOut,
	};

	typedef List<AnimationTarget>	AnimationTargetList;

	AnimationClip*			m_clip;
	AnimationTargetList		m_animationTargetList;
	double					m_currentLocalTime;
	PlayState				m_state;
	float					m_addingBlendWeight;

	InternalFade			m_internalFade;
	EasingValue<float>		m_internalFadeWeight;
};

LN_NAMESPACE_END
