
#include "../Internal.h"
#include <Lumino/Animation/A2AnimationTrack.h>
#include "VmdLoader.h"

LN_NAMESPACE_BEGIN
namespace a2
{

//==============================================================================
// AnimationValue
//==============================================================================

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
//==============================================================================
AnimationTrack::AnimationTrack(AnimationValueType type)
    : m_targetName()
    , m_type(type)
{
}

AnimationTrack::~AnimationTrack()
{
}

} // namespace a2
LN_NAMESPACE_END
