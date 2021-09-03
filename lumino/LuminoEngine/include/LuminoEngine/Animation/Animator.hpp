
#pragma once
#include "Common.hpp"
#include "AnimationMixer.hpp"
#include "../Scene/Component.hpp"

namespace ln {

#if 0
class Animator
	: public Component
	, public detail::IAnimationControllerHolder
{
public:
    /** Animator を作成します。 */
    static Ref<Animator> create();

	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip);

    void setState(const StringRef& stateName, float duration = 0.0f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(stateName, duration); }

LN_CONSTRUCT_ACCESS:
	void init();

private:
	virtual void onUpdate(float elapsedSeconds) override;
	virtual void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link) override;

	Ref<AnimationMixerCore> m_core;
};
#endif

} // namespace ln
