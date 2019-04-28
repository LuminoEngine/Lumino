
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectModel.hpp>

namespace ln {

//==============================================================================
// EffectModel

EffectModel::EffectModel()
{
}

EffectModel::~EffectModel()
{
}

void EffectModel::init()
{
    Object::init();
}

void EffectModel::onDraw(RenderingContext* context)
{
}

//==============================================================================
// SpriteFrameEffectModel

SpriteFrameEffectModel::SpriteFrameEffectModel()
{
}

SpriteFrameEffectModel::~SpriteFrameEffectModel()
{
}

void SpriteFrameEffectModel::init()
{
    EffectModel::init();
}

void SpriteFrameEffectModel::onDraw(RenderingContext* context)
{
}

} // namespace ln
