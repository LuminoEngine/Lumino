
#include "Internal.hpp"
#include <LuminoEngine/Visual/VisualComponent.hpp>
#include <LuminoEngine/Scene/VisualObject.hpp>

namespace ln {

//==============================================================================
// VisualObject

LN_OBJECT_IMPLEMENT(VisualObject, WorldObject);

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
}

void VisualObject::initialize()
{
    WorldObject::initialize();
}

void VisualObject::setVisible(bool value)
{
    mainVisualComponent()->setVisible(value);
}

bool VisualObject::isVisible() const
{
    return mainVisualComponent()->isVisible();
}

void VisualObject::setBlendMode(const Optional<BlendMode>& mode)
{
    mainVisualComponent()->setBlendMode(mode);
}

const Optional<BlendMode>& VisualObject::blendMode() const
{
    return mainVisualComponent()->blendMode();
}

void VisualObject::setOpacity(float value)
{
    mainVisualComponent()->setOpacity(value);
}

float VisualObject::opacity() const
{
    return mainVisualComponent()->opacity();
}

void VisualObject::setColorScale(const Color& value)
{
    mainVisualComponent()->setColorScale(value);
}

const Color& VisualObject::colorScale() const
{
    return mainVisualComponent()->colorScale();
}

void VisualObject::setBlendColor(const Color& value)
{
    mainVisualComponent()->setBlendColor(value);
}

const Color& VisualObject::blendColor() const
{
    return mainVisualComponent()->blendColor();
}

void VisualObject::setTone(const ToneF& value)
{
    mainVisualComponent()->setTone(value);
}

const ToneF& VisualObject::tone() const
{
    return mainVisualComponent()->tone();
}

void VisualObject::setMainVisualComponent(VisualComponent* component)
{
    m_component = component;
}

} // namespace ln

