
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Animation/Animator.hpp>


namespace ln {

#if 0
//==============================================================================
// Animator

Ref<Animator> Animator::create()
{
    return makeObject<Animator>();
}

void Animator::init()
{
	Component::init();
	m_core = makeObject<AnimationMixerCore>(this);

	// TODO: とりあえずの 1 こ
	m_core->addLayer(makeObject<AnimationLayer>(m_core));
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
            data->name = path.m_propertyName;
			data->propertyRef = prop;
			// TODO: setValueType
			m_core->addElementBlendLink(data);
		}
	}

	return m_core->addClip(stateName, animationClip);
}

void Animator::onUpdate(float elapsedSeconds)
{
	// TODO: onPreUpdate がいい。
	m_core->advanceTime(elapsedSeconds);
}

void Animator::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* link)
{
    if (!link->propertyRef.isNull()) {
        auto pair = link->propertyRef.resolve();
        pair.second->setValue(pair.first, makeVariant(std::round(link->rootValue.getFloat())));	// TODO: Variant Pool
    }
}
#endif

} // namespace ln
