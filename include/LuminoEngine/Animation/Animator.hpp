
#pragma once
#include "Common.hpp"
#include "AnimationController.hpp"
#include "../Scene/Component.hpp"

namespace ln {

class Animator
	: public Component
	, public detail::IAnimationControllerHolder
{
public:
	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip);

    void play(const StringRef& clipName, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(clipName, duration); }

LN_CONSTRUCT_ACCESS:
	void init();

private:
	virtual void onUpdate(float elapsedSeconds) override;
	virtual void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link) override;

	Ref<AnimationControllerCore> m_core;
};

} // namespace ln
