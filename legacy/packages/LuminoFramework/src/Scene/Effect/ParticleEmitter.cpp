#include "Internal.hpp"
#include <LuminoFramework/Scene/Effect/ParticleEmitterComponent.hpp>
#include <LuminoFramework/Scene/Effect/ParticleEmitter.hpp>

namespace ln {

//==============================================================================
// ParticleEmitter

ParticleEmitter::ParticleEmitter()
	: m_component(nullptr)
{
}

bool ParticleEmitter::init(ParticleModel* model)
{
	if (!VisualObject::init()) return false;

	m_component = makeObject_deprecated<ParticleEmitterComponent2>(model);
	addComponent(m_component);
	setMainVisualComponent(m_component);

	return true;
}

} // namespace ln

