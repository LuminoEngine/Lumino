
#include "../Internal.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "AnimationState.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimationState
//==============================================================================

//------------------------------------------------------------------------------
AnimationState::AnimationState(AnimationClip* clip)
	: m_clip(nullptr)
	, m_animationTargetList()
	, m_currentLocalTime(0.0)
	, m_state(PlayState_Stopped)
	, m_addingBlendWeight(1.0f)
	, m_internalFade(InternalFade::None)
	, m_internalFadeWeight()
{
	LN_REFOBJ_SET(m_clip, clip);
}

//------------------------------------------------------------------------------
AnimationState::~AnimationState()
{
	clearTargetList();
	LN_SAFE_RELEASE(m_clip);
}

//------------------------------------------------------------------------------
const String& AnimationState::getName() const
{
	return m_clip->getName();
}

//------------------------------------------------------------------------------
void AnimationState::refresh(Animator* animator)
{
	if (LN_CHECK_ARG(animator != nullptr)) return;

	clearTargetList();

	// Curve の適用先を element から探し、見つかれば t に持っておく
	for (const AnimationClip::AnimationCurveEntry& e : m_clip->getAnimationCurveEntryList())
	{
		detail::AnimationTargetAttributeEntity* target = animator->findAnimationTargetAttributeEntity(e.RelativePath);
		if (target != NULL)
		{
			AnimationTarget t;
			t.Curve = e.Curve;
			t.Target = target;
			LN_SAFE_ADDREF(t.Curve);
			m_animationTargetList.add(t);
		}
	}
}

//------------------------------------------------------------------------------
void AnimationState::setPlayState(PlayState state)
{
	// 新しく再生を開始する場合は時間をリセットする
	if (m_state == PlayState_Stopped && state == PlayState_Playing)
	{
		m_internalFade = InternalFade::None;
		m_addingBlendWeight = 0.0f;
	}

	m_state = state;
}

//------------------------------------------------------------------------------
void AnimationState::advanceTime(double elapsedTime)
{
	if (m_state == PlayState_Playing)
	{
		setLocalTime(m_currentLocalTime + elapsedTime);
	}
}

//------------------------------------------------------------------------------
void AnimationState::fadeInLinerInternal(float duration)
{
	setPlayState(PlayState_Playing);
	m_internalFade = InternalFade::FadeIn;
	m_internalFadeWeight.start(1.0f, duration);
}

//------------------------------------------------------------------------------
void AnimationState::fadeOutLinerInternal(float duration)
{
	if (m_state == PlayState_Playing)
	{
		m_internalFade = InternalFade::FadeOut;
		m_internalFadeWeight.start(0.0f, duration);
	}
}

//------------------------------------------------------------------------------
void AnimationState::clearTargetList()
{
	for (AnimationTarget& target : m_animationTargetList)
	{
		target.Curve->release();
	}
	m_animationTargetList.clear();
}

//------------------------------------------------------------------------------
void AnimationState::setLocalTime(double time)
{
	float elapsed = time - m_currentLocalTime;

	// 時間が巻き戻ったらいろいろリセット
	if (elapsed < 0.0f)
	{
		m_internalFade = InternalFade::None;
		m_addingBlendWeight = 0.0f;
	}
	else
	{
		m_internalFadeWeight.advanceTime(elapsed);
		m_addingBlendWeight = m_internalFadeWeight.getValue();
		if (m_internalFade == InternalFade::FadeOut && m_internalFadeWeight.isFinished())
		{
			// フェードアウト中で最後までたどり着いたら停止状態にする
			m_state = PlayState_Stopped;
		}
	}

	

	m_currentLocalTime = time;

	for (AnimationTarget& target : m_animationTargetList)
	{
		// 時間をセットして値を生成する
		target.Curve->getTime(time);

		// 値の型に応じてブレンド率を加味し、出力する
		target.Target->Type = target.Curve->getValueType();
		switch (target.Target->Type)
		{
		case ValueType_Float:
		{
			*((float*)target.Target->Buffer) = static_cast<FloatAnimationCurve*>(target.Curve)->getValue();
			break;
		}
		case ValueType_Vector3:
		{
			LN_THROW(0, NotImplementedException);
			break;
		}
		case ValueType_Quaternion:
		{
			LN_THROW(0, NotImplementedException);
			break;
		}
		case ValueType_SQTTransform:
		{
			if (m_addingBlendWeight != 1.0f)
			{
				AttitudeTransform v = static_cast<VMDBezierAttitudeTransformAnimation*>(target.Curve)->getValue();
				AttitudeTransform* t = (AttitudeTransform*)target.Target->Buffer;
				t->scale += v.scale * m_addingBlendWeight;
				t->rotation *= Quaternion::slerp(Quaternion::Identity, v.rotation, m_addingBlendWeight);
				t->translation += v.translation * m_addingBlendWeight;
			}
			else
			{
				*((AttitudeTransform*)target.Target->Buffer) = static_cast<VMDBezierAttitudeTransformAnimation*>(target.Curve)->getValue();
			}
			target.Target->Modified = true;	// 値をセットした
			break;
		}
		//case ValueType_Event:
		//	// 値型ではないので何もしない
		//	break;
		default:
			break;
		}
	}
}

LN_NAMESPACE_END
