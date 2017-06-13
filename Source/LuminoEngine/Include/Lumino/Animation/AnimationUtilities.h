
#pragma once
#include <functional>
#include "Common.h"
#include "EasingFunctions.h"

LN_NAMESPACE_BEGIN

class AnimationUtilities
{
public:

	typedef std::function< float(float, float, float, float) >	FloatEasingFunction;

	template<typename TValue, typename TTime>
	static std::function< TValue(TTime, TValue, TValue, TTime) > SelectEasingFunction(EasingMode mode)
	{
		typedef TValue(*EasingFunctionPtr)(TTime, TValue, TValue, TTime);
		EasingFunctionPtr table[] =
		{
			EasingFunctions::linearTween,
			EasingFunctions::easeInQuad,
			EasingFunctions::easeOutQuad,
			EasingFunctions::easeInOutQuad,
			EasingFunctions::easeInCubic,
			EasingFunctions::easeOutCubic,
			EasingFunctions::easeInOutCubic,
			EasingFunctions::easeInQuart,
			EasingFunctions::easeOutQuart,
			EasingFunctions::easeInOutQuart,
			EasingFunctions::easeInQuint,
			EasingFunctions::easeOutQuint,
			EasingFunctions::easeInOutQuint,
			EasingFunctions::easeInSine,
			EasingFunctions::easeOutSine,
			EasingFunctions::easeInOutSine,
			EasingFunctions::easeInExpo,
			EasingFunctions::easeOutExpo,
			EasingFunctions::easeInOutExpo,
			EasingFunctions::easeInCirc,
			EasingFunctions::easeOutCirc,
			EasingFunctions::easeInOutCirc,
		};
		assert(LN_ARRAY_SIZE_OF(table) == EasingMode::getMemberCount());
		return table[mode];
	}


};

LN_NAMESPACE_END
