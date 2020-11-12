
#pragma once
#include "../Mesh/Common.hpp"

namespace ln {
class AnimationClip;

/** アニメーションの繰り返し方法 */
LN_ENUM()
enum class AnimationWrapMode
{
    /** 繰り返しを行わず、1度だけ再生します。 */
    Once,

    /** 最後まで再生された後、先頭に戻ってループします。 */
    Loop,

    /** 最後まで再生された後、逆方向に戻ってループします。 */
    Alternate,
};

/** 階層構造を持つアニメーションデータの動作モード */
LN_ENUM()
enum class HierarchicalAnimationMode
{
	/** ルートノードのみ、平行移動を有効化します。 */
	AllowTranslationOnlyRoot,

	/** AllowTranslationOnlyRootY */
	AllowTranslationOnlyRootY,

	/** すべてのノードの平行移動を有効化します。 */
	AllowTranslation,

	/** 平行移動を無効化します。 */
	DisableTranslation,
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

enum class TranslationClass
{
	Absolute,
	Ratio,
};


struct AnimationTrackTargetKey
{
	HumanoidBones bone = HumanoidBones::None;
	String name;

	static bool equals(const AnimationTrackTargetKey& a, const AnimationTrackTargetKey& b)
	{
		return (a.bone != HumanoidBones::None && a.bone == b.bone) || a.name == b.name;
	}
};

namespace detail {
class AnimationManager;
} // namespace detail
} // namespace ln
