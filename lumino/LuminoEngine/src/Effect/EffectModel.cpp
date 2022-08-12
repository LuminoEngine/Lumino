
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectModel.hpp>

namespace ln {

//==============================================================================
// EffectResource

EffectResource::EffectResource()
{
}

EffectResource::~EffectResource()
{
}

Result EffectResource::init() {
    return Object::init();
}

//void EffectResource::onDraw(RenderingContext* context)
//{
//}


//==============================================================================
// EffectEmitter

EffectEmitter::EffectEmitter()
    : m_model(nullptr)
    , m_localTime(0.0f)
{
}

EffectEmitter::~EffectEmitter()
{
}

void EffectEmitter::init(EffectResource* model)
{
    Object::init();
    m_model = model;
}

bool EffectEmitter::update(float elapsedSeconds)
{
    m_localTime += elapsedSeconds;
    return onUpdate(m_localTime, elapsedSeconds);
}

void EffectEmitter::render(RenderingContext* renderingContext)
{
    onRender(renderingContext);
}

bool EffectEmitter::onUpdate(float localTime, float elapsedSeconds)
{
    return false;
}

void EffectEmitter::onRender(RenderingContext* renderingContext)
{
}

} // namespace ln
