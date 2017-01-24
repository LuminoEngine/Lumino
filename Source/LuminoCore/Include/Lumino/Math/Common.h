
#ifndef LUMINO_MATH_COMMON_H
#define LUMINO_MATH_COMMON_H

#include <Lumino/Base/Common.h>
	
LN_NAMESPACE_BEGIN

/** 回転順序 */
enum class RotationOrder
{
	XYZ,			/**< X → Y → Z */
	YZX,			/**< Y → Z → X */
	ZXY,			/**< Z → X → Y (RotationYawPitchRoll) */
};

/** 錐台の各面 */
enum class FrustumPlane
{
	Left	= 0,
	Right	= 1,
	Top		= 2,
	Bottom	= 3,
	Near	= 4,
	Far		= 5,
};

LN_NAMESPACE_END

#endif // LUMINO_MATH_COMMON_H
