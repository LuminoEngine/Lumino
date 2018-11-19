
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Visual/VisualComponent.hpp>
#include "../Rendering/RenderStage.hpp"

namespace ln {

//=============================================================================
// VisualComponent

LN_OBJECT_IMPLEMENT(VisualComponent, Component);

VisualComponent::VisualComponent()
    : m_geometryStageParameters(std::make_unique<detail::GeometryStageParameters>())    // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    , m_builtinEffectData(std::make_unique<detail::BuiltinEffectData>())    // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    , m_isVisible(true)
{
}

VisualComponent::~VisualComponent()
{
}

void VisualComponent::initialize()
{
    Component::initialize();
}

void VisualComponent::setBlendMode(const Optional<BlendMode>& mode)
{
    m_geometryStageParameters->m_blendMode = mode;
}

const Optional<BlendMode>& VisualComponent::blendMode() const
{
    return m_geometryStageParameters->m_blendMode;
}

void VisualComponent::setOpacity(float value)
{
    m_builtinEffectData->opacity = value;
}

float VisualComponent::opacity() const
{
    return m_builtinEffectData->opacity;
}

void VisualComponent::render(RenderingContext* context)
{
    if (m_isVisible)
    {
        context->pushState();

        context->setBaseTransfrom(worldObject()->worldMatrix());
        context->setBaseBuiltinEffectData(*m_builtinEffectData);
        context->setBlendMode(m_geometryStageParameters->m_blendMode);
        context->setCullingMode(m_geometryStageParameters->m_cullingMode);
        context->setDepthTestEnabled(m_geometryStageParameters->m_depthTestEnabled);
        context->setDepthWriteEnabled(m_geometryStageParameters->m_depthWriteEnabled);
        // TODO: setMaterial
        onRender(context);

        context->popState();	// TODO: scoped
    }
}

} // namespace ln

