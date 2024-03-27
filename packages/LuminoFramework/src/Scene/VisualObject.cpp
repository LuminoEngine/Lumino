#include "Internal.hpp"
#include <LuminoFramework/Visual/VisualComponent.hpp>
#include <LuminoFramework/Scene/VisualObject.hpp>

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

void VisualObject::setShadingModel(const Optional_deprecated<ShadingModel>& value)
{
    mainVisualComponent()->setShadingModel(value);
}

const Optional_deprecated<ShadingModel>& VisualObject::shadingModel() const
{
    return mainVisualComponent()->shadingModel();
}

void VisualObject::setBlendMode(const Optional_deprecated<BlendMode>& mode)
{
    mainVisualComponent()->setBlendMode(mode);
}

const Optional_deprecated<BlendMode>& VisualObject::blendMode() const
{
    return mainVisualComponent()->blendMode();
}

void VisualObject::setDepthTestEnabled(const Optional_deprecated<bool>& enabled)
{
    mainVisualComponent()->setDepthTestEnabled(enabled);
}

const Optional_deprecated<bool>& VisualObject::isDepthTestEnabled() const
{
    return mainVisualComponent()->isDepthTestEnabled();
}

void VisualObject::setDepthWriteEnabled(const Optional_deprecated<bool>& enabled)
{
    mainVisualComponent()->setDepthWriteEnabled(enabled);
}

const Optional_deprecated<bool>& VisualObject::isDepthWriteEnabled() const
{
    return mainVisualComponent()->isDepthWriteEnabled();
}

void VisualObject::setCullMode(const Optional_deprecated<CullMode>& mode)
{
    mainVisualComponent()->setCullMode(mode);
}

const Optional_deprecated<CullMode>& VisualObject::cullMode() const
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

void VisualObject::serialize_deprecated(Serializer2_deprecated& ar)
{
    WorldObject::serialize_deprecated(ar);
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

