#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/GPU/SwapChain.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>

namespace ln {

Ref<SurfaceContext> SurfaceContext::createFromWindow(detail::RenderingManager* renderingManager, PlatformWindow* window) {
    GraphicsManager* graphicsManager = renderingManager->graphicsManager();
    Ref<SurfaceContext> context = makeObject_deprecated<SurfaceContext>();
    context->m_context = graphicsManager->createGraphicsContext(window);
    context->m_renderingContext = makeObject_deprecated<CommandList>();
    context->m_drawEventList = makeURef<kanata::DrawEventList>(renderingManager);
    return context;
}

Ref<SurfaceContext> SurfaceContext::createFromExternal(
    detail::RenderingManager* renderingManager,
    GraphicsContext* externalContext) {
    Ref<SurfaceContext> context = makeObject_deprecated<SurfaceContext>();
    context->m_context = externalContext;
    context->m_renderingContext = makeObject_deprecated<CommandList>();
    context->m_drawEventList = makeURef<kanata::DrawEventList>(renderingManager);
    return context;
}

SurfaceContext::SurfaceContext()
    : m_context(nullptr) 
    , m_renderingContext()
    , m_drawEventList() {
}

SurfaceContext::~SurfaceContext() {
}

GraphicsCommandList* SurfaceContext::commandList() const {
    return m_context->currentCommandList2();
}

} // namespace ln

