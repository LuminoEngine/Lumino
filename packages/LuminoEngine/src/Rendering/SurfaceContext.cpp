#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GPU/SwapChain.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>

namespace ln {

Result_deprecated<Ref<SurfaceContext>> SurfaceContext::createFromWindow(detail::RenderingManager* renderingManager, PlatformWindow* window) {
    GraphicsManager* graphicsManager = renderingManager->graphicsManager();
    Ref<SurfaceContext> context = makeObject_deprecated<SurfaceContext>();

    auto result = graphicsManager->createGraphicsContext(window);
    if (!result) {
        return result;
    }
    context->m_ownerWindowOrNull = window;
    context->m_context = result.unwrap();
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
    : m_ownerWindowOrNull(nullptr)
    , m_context(nullptr) 
    , m_renderingContext()
    , m_drawEventList()
    , m_frameTimer()
    , m_lastFrameTime(0.0f) {
}

SurfaceContext::~SurfaceContext() {
}

GraphicsContext* SurfaceContext::context() const {
    return m_context;
}

GraphicsCommandList* SurfaceContext::commandList() const {
    return m_context->currentCommandList2();
}

CommandList* SurfaceContext::renderingContext() const {
    return m_renderingContext;
}

kanata::DrawEventList* SurfaceContext::drawEventList() const {
    return m_drawEventList;
}

void SurfaceContext::beginFrame() {
    m_frameTimer.start();
}

void SurfaceContext::endFrame() {
    m_lastFrameTime = static_cast<float>(m_frameTimer.elapsed());
}

} // namespace ln

