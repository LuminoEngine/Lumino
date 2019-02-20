
#pragma once
#include "Common.hpp"
#include "AnimationController.hpp"
#include "../Scene/Component.hpp"

namespace ln {

class Animator
	: public Component
	, public detail::IAnimationController
{
public:
	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip);

LN_CONSTRUCT_ACCESS:
	void init();

private:
	virtual detail::AnimationTargetElementBlendLink* findAnimationTargetElementBlendLink(const StringRef& name) override;

	List<Ref<AnimationLayer>> m_layers;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_targetElementBlendLinks;
};

} // namespace ln
