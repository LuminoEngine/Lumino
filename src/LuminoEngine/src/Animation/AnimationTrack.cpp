
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>

namespace ln {

//==============================================================================
// AnimationValue

AnimationValue::AnimationValue()
	: m_type(AnimationValueType::Null)
{
}

AnimationValue::AnimationValue(AnimationValueType type)
    : AnimationValue()
{
	resetType(type);
}

void AnimationValue::resetType(AnimationValueType type)
{
	m_type = type;
	clearValue();
}

void AnimationValue::clearValue()
{
	switch (m_type)
	{
	case AnimationValueType::Float:
		v_Float = 0.0f;
		break;
	case AnimationValueType::Vector3:
		v_Vector3 = Vector3();
		break;
	case AnimationValueType::Quaternion:
		v_Quaternion = Quaternion();
		break;
	case AnimationValueType::Transform:
		v_Transform.scale = Vector3();	// 補間した値にウェイトかけてどんどん足していくので 0 で初期化しておく
		v_Transform.rotation = Quaternion();
		v_Transform.translation = Vector3();
		break;
	default:
		break;
	}
}

//==============================================================================
// AnimationTrack

AnimationTrack::AnimationTrack(AnimationValueType type)
    : m_targetName()
    , m_type(type)
{
}

AnimationTrack::~AnimationTrack()
{
}

void AnimationTrack::init()
{
    Object::init();
}

//==============================================================================
// ScalarAnimationTrack

Ref<ScalarAnimationTrack> ScalarAnimationTrack::create()
{
    return newObject<ScalarAnimationTrack>();
}

ScalarAnimationTrack::ScalarAnimationTrack()
    : AnimationTrack(AnimationValueType::Float)
{
}

ScalarAnimationTrack::~ScalarAnimationTrack()
{
}

void ScalarAnimationTrack::init()
{
}

void ScalarAnimationTrack::setCurve(AnimationCurve* curve)
{
    m_curve = curve;
}

float ScalarAnimationTrack::lastFrameTime() const
{
    return m_curve->lastFrameTime();
}

float ScalarAnimationTrack::evaluate(float time)
{
    return m_curve->evaluate(time);
}

void ScalarAnimationTrack::evaluate(float time, AnimationValue* outResult)
{
    outResult->setFloat(m_curve->evaluate(time));
}

} // namespace ln
