
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectContext.hpp>

namespace ln {

//==============================================================================
// EffectEmitter

EffectEmitter::EffectEmitter()
    : m_model(nullptr)
{
}

EffectEmitter::~EffectEmitter()
{
}

void EffectEmitter::init(EffectModel* model)
{
    Object::init();
    m_model = model;
}

void EffectEmitter::update(float elapsedSeconds)
{
}

void EffectEmitter::render(RenderingContext* renderingContext)
{
}

//==============================================================================
// EffectContext

EffectContext::EffectContext()
    : m_manager(nullptr)
{
}

EffectContext::~EffectContext()
{
}

void EffectContext::init()
{
    Object::init();
}

EffectEmitter* EffectContext::createEmitter(EffectModel* model)
{
    // TODO: Pool

    auto emitter = newObject<EffectEmitter>(model);
    m_emitters.add(emitter);
    return emitter;
}

void EffectContext::update(float elapsedSeconds)
{
    for (auto& emitter : m_emitters) {
        emitter->update(elapsedSeconds);
    }
}

void EffectContext::render(RenderingContext* renderingContext)
{
    for (auto& emitter : m_emitters) {
        emitter->render(renderingContext);
    }
}

} // namespace ln
