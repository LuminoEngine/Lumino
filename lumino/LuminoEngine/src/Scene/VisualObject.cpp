
#include "Internal.hpp"
#include <LuminoEngine/Visual/VisualComponent.hpp>
#include <LuminoEngine/Scene/VisualObject.hpp>

namespace ln {

//==============================================================================
// VisualObject

LN_OBJECT_IMPLEMENT(VisualObject, WorldObject) {}

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
}

bool VisualObject::init()
{
    return WorldObject::init();
}

void VisualObject::setVisible(bool value)
{
    mainVisualComponent()->setVisible(value);
}

bool VisualObject::isVisible() const
{
    return mainVisualComponent()->isVisible();
}

void VisualObject::setShadingModel(const Optional<ShadingModel>& value)
{
    mainVisualComponent()->setShadingModel(value);
}

const Optional<ShadingModel>& VisualObject::shadingModel() const
{
    return mainVisualComponent()->shadingModel();
}

void VisualObject::setBlendMode(const Optional<BlendMode>& mode)
{
    mainVisualComponent()->setBlendMode(mode);
}

const Optional<BlendMode>& VisualObject::blendMode() const
{
    return mainVisualComponent()->blendMode();
}

void VisualObject::setDepthTestEnabled(const Optional<bool>& enabled)
{
    mainVisualComponent()->setDepthTestEnabled(enabled);
}

const Optional<bool>& VisualObject::isDepthTestEnabled() const
{
    return mainVisualComponent()->isDepthTestEnabled();
}

void VisualObject::setDepthWriteEnabled(const Optional<bool>& enabled)
{
    mainVisualComponent()->setDepthWriteEnabled(enabled);
}

const Optional<bool>& VisualObject::isDepthWriteEnabled() const
{
    return mainVisualComponent()->isDepthWriteEnabled();
}

void VisualObject::setCullMode(const Optional<CullMode>& mode)
{
    mainVisualComponent()->setCullMode(mode);
}

const Optional<CullMode>& VisualObject::cullMode() const
{
    return mainVisualComponent()->cullMode();
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

void VisualObject::setTone(const ColorTone& value)
{
    mainVisualComponent()->setTone(value);
}

const ColorTone& VisualObject::tone() const
{
    return mainVisualComponent()->tone();
}

void VisualObject::setRenderPriority(int value)
{
    mainVisualComponent()->setRenderPriority(value);
}

void VisualObject::serialize(Serializer2& ar)
{
    WorldObject::serialize(ar);
}

void VisualObject::setMainVisualComponent(VisualComponent* component)
{
    m_component = component;
}

//==============================================================================
// VisualObject::BuilderDetails

void VisualObject::BuilderDetails::apply(VisualObject* p) const
{
    WorldObject::BuilderDetails::apply(p);

    if (visible) p->setVisible(*visible);
    if (shadingModel) p->setShadingModel(*shadingModel);
    if (blendMode) p->setBlendMode2(*blendMode);
    if (cullMode) p->setCullMode(*cullMode);

    if (depthTest) p->setDepthTestEnabled(*depthTest);
    if (depthWrite) p->setDepthWriteEnabled(*depthWrite);

    if (opacity) p->setOpacity(*opacity);
    if (colorScale) p->setColorScale(*colorScale);
    if (blendColor) p->setBlendColor(*blendColor);
    if (colorTone) p->setTone(*colorTone);
}

} // namespace ln

