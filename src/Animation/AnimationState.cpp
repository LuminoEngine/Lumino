
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
	ClearTargetList();
	LN_SAFE_RELEASE(m_clip);
}

//------------------------------------------------------------------------------
const String& AnimationState::GetName() const
{
	return m_clip->GetName();
}

//------------------------------------------------------------------------------
void AnimationState::Refresh(Animator* animator)
{
	LN_CHECK_ARG(animator != nullptr);

	ClearTargetList();

	// Curve の適用先を element から探し、見つかれば t に持っておく
	for (const AnimationClip::AnimationCurveEntry& e : m_clip->GetAnimationCurveEntryList())
	{
		detail::AnimationTargetAttributeEntity* target = animator->FindAnimationTargetAttributeEntity(e.RelativePath);
		if (target != NULL)
		{
			AnimationTarget t;
			t.Curve = e.Curve;
			t.Target = target;
			LN_SAFE_ADDREF(t.Curve);
			m_animationTargetList.Add(t);
		}
	}
}

//------------------------------------------------------------------------------
void AnimationState::SetPlayState(PlayState state)
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
void AnimationState::AdvanceTime(double elapsedTime)
{
	if (m_state == PlayState_Playing)
	{
		SetLocalTime(m_currentLocalTime + elapsedTime);
	}
}

//------------------------------------------------------------------------------
void AnimationState::FadeInLinerInternal(float duration)
{
	SetPlayState(PlayState_Playing);
	m_internalFade = InternalFade::FadeIn;
	m_internalFadeWeight.Start(1.0f, duration);
}

//------------------------------------------------------------------------------
void AnimationState::FadeOutLinerInternal(float duration)
{
	if (m_state == PlayState_Playing)
	{
		m_internalFade = InternalFade::FadeOut;
		m_internalFadeWeight.Start(0.0f, duration);
	}
}

//------------------------------------------------------------------------------
void AnimationState::ClearTargetList()
{
	for (AnimationTarget& target : m_animationTargetList)
	{
		target.Curve->Release();
	}
	m_animationTargetList.Clear();
}

//------------------------------------------------------------------------------
void AnimationState::SetLocalTime(double time)
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
		m_internalFadeWeight.AdvanceTime(elapsed);
		m_addingBlendWeight = m_internalFadeWeight.GetValue();
		if (m_internalFade == InternalFade::FadeOut && m_internalFadeWeight.IsFinished())
		{
			// フェードアウト中で最後までたどり着いたら停止状態にする
			m_state = PlayState_Stopped;
		}
	}

	

	m_currentLocalTime = time;

	for (AnimationTarget& target : m_animationTargetList)
	{
		// 時間をセットして値を生成する
		target.Curve->SetTime(time);

		// 値の型に応じてブレンド率を加味し、出力する
		target.Target->Type = target.Curve->GetValueType();
		switch (target.Target->Type)
		{
		case ValueType_Float:
		{
			*((float*)target.Target->Buffer) = static_cast<FloatAnimationCurve*>(target.Curve)->GetValue();
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
				SQTTransform v = static_cast<VMDBezierSQTTransformAnimation*>(target.Curve)->GetValue();
				SQTTransform* t = (SQTTransform*)target.Target->Buffer;
				t->scale += v.scale * m_addingBlendWeight;
				t->rotation *= Quaternion::Slerp(Quaternion::Identity, v.rotation, m_addingBlendWeight);
				t->translation += v.translation * m_addingBlendWeight;
			}
			else
			{
				*((SQTTransform*)target.Target->Buffer) = static_cast<VMDBezierSQTTransformAnimation*>(target.Curve)->GetValue();
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
