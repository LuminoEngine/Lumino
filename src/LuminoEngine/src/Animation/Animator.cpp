﻿
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
	m_core = newObject<AnimationControllerCore>(this);

	// TODO: とりあえずの 1 こ
	m_core->addLayer(newObject<AnimationLayer>(m_core));
}

AnimationState* Animator::addClip(const StringRef& stateName, AnimationClip* animationClip)
{
	for (auto& track : animationClip->tracks())
	{
		// TODO: 今はプロパティだけ。オブジェクトもやっとく
		PropertyPath path;
		path.m_propertyName = track->targetName();
		PropertyRef prop = PropertyPath::findProperty(worldObject(), &path);

		auto* link = m_core->findAnimationTargetElementBlendLink(track->targetName());
		if (!link) {
			// 新規追加

			auto data = makeRef<detail::AnimationTargetElementBlendLink>();
			data->propertyRef = prop;
			// TODO: setValueType
			m_core->addElementBlendLink(data);
		}
	}

	return m_core->addClip(animationClip);
}

void Animator::onUpdate(float elapsedSeconds)
{
	// TODO: onPreUpdate がいい。
	m_core->advanceTime(elapsedSeconds);
}

void Animator::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link)
{
}

} // namespace ln
