#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "UIElement.hpp"

namespace ln {
class AnimationCurve;
	//enum class UIAnimationDirection
	//{
	//	/** 普通方向の再生でアニメーションサイクルを繰り返す。 */
	//	Normal,

	//	/** 奇数回では普通方向の再生、偶数回では逆方向の再生となって、アニメーションサイクルを繰り返す */
	//	Alternate,
	//};


namespace detail {
class UIStyleInstance;

enum UIStyleAnimationElement
{
	UIStyleAnimationElement_Width,
	UIStyleAnimationElement_Height,
	UIStyleAnimationElement_PositionX,
	UIStyleAnimationElement_PositionY,
	UIStyleAnimationElement_PositionZ,
	UIStyleAnimationElement_Opacity,

	UIStyleAnimationElement_Count_,
};

// finalStyle に付く
class UIStyleAnimationInstance
	: public RefObject
{
public:
	void attach(UIStyleInstance* targetStyleInstance);
	void update(float elapsedSeconds);

private:
	UIStyleInstance* m_targetStyleInstance;
};

} // namespace detail


class UIAnimationClip
	: public Object
{
public:
	void addWidth(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Width, startValue, targetValue, duration, func, wrapMode); }
	void addHeight(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Height, startValue, targetValue, duration, func, wrapMode); }
	void addPositionX(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionX, startValue, targetValue, duration, func, wrapMode); }
	void addPositionY(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionY, startValue, targetValue, duration, func, wrapMode); }
	void addPositionZ(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_PositionZ, startValue, targetValue, duration, func, wrapMode); }
	void addOpacity(float startValue, float targetValue, float duration, EasingMode func = EasingMode::Linear, AnimationWrapMode wrapMode = AnimationWrapMode::Once) { addCurve(detail::UIStyleAnimationElement_Opacity, startValue, targetValue, duration, func, wrapMode); }

LN_CONSTRUCT_ACCESS:
	UIAnimationClip();
	bool init();

private:
	void addCurve(detail::UIStyleAnimationElement element, float startValue, float targetValue, float duration, EasingMode func, AnimationWrapMode wrapMode);

	std::array<Ref<AnimationCurve>, detail::UIStyleAnimationElement_Count_> m_curves;
};

class UITransitionClip
	: public Object
{
public:
	struct Curve
	{
		EasingFunctions::FloatEasingFunctionPtr function = nullptr;
		float targetValue;
		float duration;
		float delay;
	};

	void addWidth(float targetValue, float duration, EasingMode func = EasingMode::Linear, float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Width, targetValue, duration, func, delay); }
	void addHeight(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Height, targetValue, duration, func, delay); }
	void addPositionX(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionX, targetValue, duration, func, delay); }
	void addPositionY(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionY, targetValue, duration, func, delay); }
	void addPositionZ(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_PositionZ, targetValue, duration, func, delay); }
	void addOpacity(float targetValue, float duration, EasingMode func = EasingMode::Linear,float delay = 0.0f) { addCurve(detail::UIStyleAnimationElement_Opacity, targetValue, duration, func, delay); }

LN_CONSTRUCT_ACCESS:
	UITransitionClip();
    bool init();

private:
	void addCurve(detail::UIStyleAnimationElement element, float targetValue, float duration, EasingMode func, float delay);

	std::array<Curve, detail::UIStyleAnimationElement_Count_> m_curves;
};

} // namespace ln

