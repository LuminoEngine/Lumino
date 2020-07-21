
#pragma once
#include "../Mesh/Common.hpp"

namespace ln {
class AnimationClip;

/** アニメーションの繰り返し方法 */
enum class AnimationWrapMode
{
    /** 繰り返しを行わず、1度だけ再生します。 */
    Once,

    /** 最後まで再生された後、先頭に戻ってループします。 */
    Loop,

    /** 最後まで再生された後、逆方向に戻ってループします。 */
    Alternate,
};

enum class AnimationClockAffiliation
{
    Standalone,

    ActiveWorld,
};

/** イージング関数の種類を表します。*/
enum class EasingMode
{
	Linear = 0,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
};


struct AnimationTrackTargetKey
{
	HumanoidBones bone = HumanoidBones::None;
	String name;

	static bool equals(const AnimationTrackTargetKey& a, const AnimationTrackTargetKey& b)
	{
		return a.bone == b.bone || a.name == b.name;
	}
};

namespace detail {
class AnimationManager;
} // namespace detail
} // namespace ln
