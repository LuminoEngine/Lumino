
#include "Internal.hpp"
#include <LuminoEngine/Scene/Effect/ParticleEmitterComponent.hpp>
#include <LuminoEngine/Scene/Effect/ParticleEmitter.hpp>

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

	m_component = makeObject<ParticleEmitterComponent2>(model);
	addComponent(m_component);
	setMainVisualComponent(m_component);

	return true;
}

} // namespace ln

