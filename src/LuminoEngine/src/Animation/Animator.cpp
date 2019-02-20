
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Animation/Animator.hpp>


namespace ln {

//==============================================================================
// Animator

void Animator::init()
{
	Component::init();

	// TODO: とりあえずの 1 こ
	m_layers.add(newObject<AnimationLayer>(this));
}

AnimationState* Animator::addClip(const StringRef& stateName, AnimationClip* animationClip)
{
	for (auto& track : animationClip->tracks())
	{
		// TODO: 今はプロパティだけ。オブジェクトもやっとく
		PropertyPath path;
		path.m_propertyName = track->targetName();
		PropertyRef prop = PropertyPath::findProperty(worldObject(), &path);

		auto* link = findAnimationTargetElementBlendLink(track->targetName());
		if (!link) {
			// 新規追加

			auto data = makeRef<detail::AnimationTargetElementBlendLink>();
			data->propertyRef = prop;
			// TODO: setValueType
			m_targetElementBlendLinks.add(data);
		}
	}

	return m_layers[0]->addClipAndCreateState(animationClip);
}

detail::AnimationTargetElementBlendLink* AnimationController::findAnimationTargetElementBlendLink(const StringRef& name)
{
	auto data = m_targetElementBlendLinks.findIf([name](const Ref<detail::AnimationTargetElementBlendLink>& data) { return data->name == name; });
	if (data)
		return (*data);
	else
		return nullptr;
}

} // namespace ln
