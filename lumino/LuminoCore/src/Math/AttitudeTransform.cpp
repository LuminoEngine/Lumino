
#include <LuminoCore/Math/AttitudeTransform.hpp>

namespace ln {

//==============================================================================
// AttitudeTransform

// 名前は OpenSceneGraph の PositionAttitudeTransform から。

const AttitudeTransform AttitudeTransform::Identity = AttitudeTransform();

AttitudeTransform::AttitudeTransform()
	: scale(Vector3::Ones)
	, rotation(Quaternion::Identity)
	, translation(Vector3::Zero)
{
}

AttitudeTransform::AttitudeTransform(const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_)
	: scale(scale_)
	, rotation(rotation_)
	, translation(translation_)
{
}
	
} // namespace ln
