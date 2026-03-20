
#pragma once
#include "../Common.hpp"
#include "../VisualObject.hpp"

namespace ln {
class ParticleModel;
    
/**
 * ParticleEmitter
 */
LN_CLASS()
class ParticleEmitter
    : public VisualObject
{
public:

LN_CONSTRUCT_ACCESS:
    ParticleEmitter();

	/** init */
	LN_METHOD()
    bool init(ParticleModel* model);

private:
    Ref<ParticleEmitterComponent2> m_component;
};

} // namespace ln
