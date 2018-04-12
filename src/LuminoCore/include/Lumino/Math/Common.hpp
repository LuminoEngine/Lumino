// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

#include <Lumino/Base/Common.hpp>

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

} // namespace ln
