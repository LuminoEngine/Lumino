// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include <assert.h>
#include <ostream>
#include "../Base/Common.hpp"
#include "../Base/String.hpp"

namespace ln {

/** 回転順序 */
enum class RotationOrder
{
    /** X → Y → Z */
    XYZ,
	
    /** Y → Z → X */
    YZX,

    /** Z → X → Y (RotationYawPitchRoll) */
    ZXY,
};

/** 錐台の各面 */
enum class FrustumPlane
{
    Left = 0,
    Right = 1,
    Top = 2,
    Bottom = 3,
    Near = 4,
    Far = 5,
};

namespace detail {

class MathHelper
{
public:
	static std::ostream& outputStream(std::ostream& s, const float* elements, int count)
	{
		s << elements[0];
		for (int i = 1; i < count; i++) {
			s << ' ' << elements[i];
		}
		return s;
	}
};

} // namespace detail
} // namespace ln
