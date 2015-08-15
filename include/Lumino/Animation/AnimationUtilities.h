
#pragma once
#include <functional>
#include "Common.h"
#include "EasingFunctions.h"

namespace Lumino
{

class AnimationUtilities
{
public:
	template<typename T>
	std::function< T(T, T, T, T) > SelectEasingFunction(Animation::EasingMode mode)
	{
		const std::function< T(T, T, T, T) > table[] =
		{
			EasingFunctions::LinearTween,
			EasingFunctions::EaseInQuad,
			EasingFunctions::EaseOutQuad,
			EasingFunctions::EaseInOutQuad,
			EasingFunctions::EaseInCubic,
			EasingFunctions::EaseOutCubic,
			EasingFunctions::EaseInOutCubic,
			EasingFunctions::EaseInQuart,
			EasingFunctions::EaseOutQuart,
			EasingFunctions::EaseInOutQuart,
			EasingFunctions::EaseInQuint,
			EasingFunctions::EaseOutQuint,
			EasingFunctions::EaseInOutQuint,
			EasingFunctions::EaseInSine,
			EasingFunctions::EaseOutSine,
			EasingFunctions::EaseInOutSine,
			EasingFunctions::EaseInExpo,
			EasingFunctions::EaseOutExpo,
			EasingFunctions::EaseInOutExpo,
			EasingFunctions::EaseInCirc,
			EasingFunctions::EaseOutCirc,
			EasingFunctions::EaseInOutCirc,
		};
		assert(LN_ARRAY_SIZEOF(table) == Animation::EasingMode::GetMemberCount());
		return table[mode];
	}


};

} // namespace Lumino
