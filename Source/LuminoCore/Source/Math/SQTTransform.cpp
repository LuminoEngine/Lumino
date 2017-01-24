
#include "../../include/Lumino/Math/SQTTransform.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SQTTransform
//==============================================================================

const SQTTransform SQTTransform::Identity = SQTTransform();

//------------------------------------------------------------------------------
SQTTransform::SQTTransform()
	: scale(Vector3::Ones)
	, rotation(Quaternion::Identity)
	, translation(Vector3::Zero)
{
}

//------------------------------------------------------------------------------
SQTTransform::SQTTransform(const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_)
	: scale(scale_)
	, rotation(rotation_)
	, translation(translation_)
{
}
	
LN_NAMESPACE_END
