
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Visual/VisualComponent.hpp>
#include "../../Graphics/src/Rendering/RenderStage.hpp"

namespace ln {

//==============================================================================
// VisualComponent

LN_OBJECT_IMPLEMENT(VisualComponent, Component) {}

VisualComponent::VisualComponent()
    : m_geometryStageParameters(std::make_unique<detail::GeometryStageParameters>())    // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    , m_builtinEffectData(std::make_unique<detail::BuiltinEffectData>())    // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    , m_renderPriority(0)
    , m_isVisible(true)
{
}

VisualComponent::~VisualComponent()
{
}

bool VisualComponent::init()
{
    return Component::init();
}

void VisualComponent::setBlendMode(const Optional_deprecated<BlendMode>& mode)
{
    m_geometryStageParameters->m_blendMode = mode;
}

const Optional_deprecated<BlendMode>& VisualComponent::blendMode() const
{
    return m_geometryStageParameters->m_blendMode;
}

void VisualComponent::setShadingModel(const Optional_deprecated<ShadingModel>& value)
{
    m_geometryStageParameters->shadingModel = value;
}

const Optional_deprecated<ShadingModel>& VisualComponent::shadingModel() const
{
    return m_geometryStageParameters->shadingModel;
}

void VisualComponent::setDepthTestEnabled(const Optional_deprecated<bool>& enabled)
{
    m_geometryStageParameters->m_depthTestEnabled = enabled;
}

const Optional_deprecated<bool>& VisualComponent::isDepthTestEnabled() const
{
    return m_geometryStageParameters->m_depthTestEnabled;
}

void VisualComponent::setDepthWriteEnabled(const Optional_deprecated<bool>& enabled)
{
    m_geometryStageParameters->m_depthWriteEnabled = enabled;
}

const Optional_deprecated<bool>& VisualComponent::isDepthWriteEnabled() const
{
    return m_geometryStageParameters->m_depthWriteEnabled;
}

void VisualComponent::setCullMode(const Optional_deprecated<CullMode>& mode)
{
    m_geometryStageParameters->m_cullingMode = mode;
}

const Optional_deprecated<CullMode>& VisualComponent::cullMode() const
{
    return m_geometryStageParameters->m_cullingMode;
}

void VisualComponent::setOpacity(float value)
{
    m_builtinEffectData->opacity = value;
}

float VisualComponent::opacity() const
{
    return m_builtinEffectData->opacity;
}

void VisualComponent::setColorScale(const Color& value)
{
    m_builtinEffectData->colorScale = value;
}

const Color& VisualComponent::colorScale() const
{
    return m_builtinEffectData->colorScale;
}

void VisualComponent::setBlendColor(const Color& value)
{
    m_builtinEffectData->blendColor = value;
}

const Color& VisualComponent::blendColor() const
{
    return m_builtinEffectData->blendColor;
}

void VisualComponent::setTone(const ColorTone& value)
{
    m_builtinEffectData->tone = value;
}

const ColorTone& VisualComponent::tone() const
{
    return m_builtinEffectData->tone;
}

void VisualComponent::setRenderPriority(int value)
{
    m_renderPriority = value;
}

void VisualComponent::render(RenderingContext* context)
{
    if (m_isVisible)
    {
        context->pushState();

        context->setRenderPriority(m_renderPriority);
        context->setBaseTransfrom(worldObject()->worldMatrix());
        context->setBaseBuiltinEffectData(*m_builtinEffectData);
        context->setBlendMode(m_geometryStageParameters->m_blendMode);
        context->setShadingModel(m_geometryStageParameters->shadingModel);
        context->setCullingMode(m_geometryStageParameters->m_cullingMode);
        context->setDepthTestEnabled(m_geometryStageParameters->m_depthTestEnabled);
        context->setDepthWriteEnabled(m_geometryStageParameters->m_depthWriteEnabled);
        // TODO: setMaterial
        onRender(context);

        context->popState();	// TODO: scoped
    }
}

} // namespace ln

