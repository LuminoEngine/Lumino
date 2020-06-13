
#include "../Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include <LuminoEngine/Scene/Reflection/ReflectorComponent.hpp>

namespace ln {

//=============================================================================
// ReflectorComponent

LN_OBJECT_IMPLEMENT(ReflectorComponent, VisualComponent) {}

ReflectorComponent::ReflectorComponent()
    : m_material(nullptr)
    , m_size(10.0f, 10.0f)
{
}

bool ReflectorComponent::init()
{
	if (!VisualComponent::init()) return false;

    m_renderTarget = makeObject<RenderTargetTexture>(512, 512, TextureFormat::RGBA8, false);

    m_offscreenRenderView = makeObject<OffscreenWorldRenderView>();
    m_offscreenRenderView->setRenderTarget(m_renderTarget);

    m_material = makeObject<Material>();
    //m_material->setColor(Color::Red);
    m_material->setMainTexture(m_renderTarget);

	return true;
}

void ReflectorComponent::onPrepareRender(RenderingContext* context)
{
    const auto* viewPoint = context->viewPoint();
    auto& camera = m_offscreenRenderView->m_cameraInfo;
    {
        camera.viewPixelSize = Size(m_renderTarget->width(), m_renderTarget->height());
        camera.viewPosition = viewPoint->viewPosition;
        camera.viewDirection = viewPoint->viewDirection;
        camera.viewMatrix = viewPoint->viewMatrix;
        camera.projMatrix = viewPoint->projMatrix;
        camera.viewProjMatrix = viewPoint->viewProjMatrix;
        camera.viewFrustum = viewPoint->viewFrustum;
        camera.nearClip = viewPoint->nearClip;
        camera.farClip = viewPoint->farClip;

    }

    context->world->enqueueOffscreenRenderView(m_offscreenRenderView);
}

void ReflectorComponent::onRender(RenderingContext* context)
{
    if (context->currentRenderView == m_offscreenRenderView) {

    }
    else {

        context->setMaterial(m_material);

        auto uv1 = Vector2(1.0, 1.0);

        context->drawPlane(m_size.x, m_size.y, Vector2::Zero, uv1, Color::White);
    }
}

} // namespace ln

