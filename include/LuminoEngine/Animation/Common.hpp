
#pragma once

namespace ln {

/** アニメーションの繰り返し方法 */
enum class AnimationWrapMode
{
    /** 繰り返しを行わず、1度だけ再生します。 */
    Once,

    /** 最後まで再生された後、先頭に戻ってループします。 */
    Loop,

    /** 最後まで再生された後、逆方向に戻ってループします。 */
    RoundTrip,
};

enum class AnimationClockAffiliation
{
    Standalone,

    ActiveWorld,
};

} // namespace ln
