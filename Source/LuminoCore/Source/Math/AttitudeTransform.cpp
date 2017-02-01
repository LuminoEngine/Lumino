
#include <Lumino/Math/AttitudeTransform.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// AttitudeTransform
//==============================================================================
// 名前は OpenSceneGraph の PositionAttitudeTransform から。

const AttitudeTransform AttitudeTransform::Identity = AttitudeTransform();

//------------------------------------------------------------------------------
AttitudeTransform::AttitudeTransform()
	: scale(Vector3::Ones)
	, rotation(Quaternion::Identity)
	, translation(Vector3::Zero)
{
}

//------------------------------------------------------------------------------
AttitudeTransform::AttitudeTransform(const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_)
	: scale(scale_)
	, rotation(rotation_)
	, translation(translation_)
{
}
	
LN_NAMESPACE_END
