#include "Internal.hpp"
#include <LuminoEngine/GPU/detail/GraphicsResourceRegistry.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>
#include <LuminoEngine/GPU/SwapChain.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/ShaderDescriptor.hpp>
#include "RenderTargetTextureCache.hpp"
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/GPU/detail/RenderingCommandList.hpp>
#include "RenderPassCache.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// SwapChain

GraphicsContext::GraphicsContext()
    : m_manager(nullptr)
    , m_rhiObject(nullptr)
    , m_rhiResourceRegistry()
    , m_backbuffers()
    , m_imageIndex(-1) {
}

GraphicsContext::~GraphicsContext() {
}

void GraphicsContext::init(PlatformWindow* window) {
    // TODO: onChangeDevice でバックバッファをアタッチ
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

    m_rhiResourceRegistry = makeURef<detail::RHIGraphicsResourceRegistry>(m_manager->resourceRegistry());

    SizeI backbufferSize;
    window->getFramebufferSize(&backbufferSize.width, &backbufferSize.height);

    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createSwapChain(window, backbufferSize);

    resetRHIBackbuffers();

    // CommandList
    uint32_t count = m_rhiObject->getBackbufferCount();
    m_commandLists.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        auto commandList = makeRef<GraphicsCommandList>();
        commandList->init(detail::GraphicsResourceInternal::manager(this));
        m_commandLists[i] = commandList;
    }

    nextFrame();
}

void GraphicsContext::onDispose(bool explicitDisposing) {
    //if (!m_commandLists.empty()) {
    //    // End command list
    //    detail::GraphicsCommandListInternal::endCommandRecoding(currentCommandList2());
    //}

    m_rhiObject = nullptr;
    for (auto& x : m_commandLists)
        x->dispose();
    m_commandLists.clear();
    m_depthBuffers.clear();
    m_backbuffers.clear();

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void GraphicsContext::onChangeDevice(detail::IGraphicsDevice* device) {
}

Size GraphicsContext::backbufferSize() const {
    const RenderTargetTexture* backbuffers = m_backbuffers[0];
    return Size(backbuffers->width(), backbuffers->height());
}

void GraphicsContext::resizeBackbuffer(int width, int height) {
    GraphicsCommandList* commandList = currentCommandList2();
    if (commandList) {
        LN_ASSERT(commandList->m_scopeState == GraphicsCommandList::ScopeState::Idle);
    }

    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
    resetRHIBackbuffers();
    nextFrame();
}

RenderTargetTexture* GraphicsContext::currentBackbuffer() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_backbuffers[m_imageIndex];
}

GraphicsCommandList* GraphicsContext::currentCommandList2() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_commandLists[m_imageIndex];
}

RenderPass* GraphicsContext::currentRenderPass() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_renderPasses[m_imageIndex];
}

void GraphicsContext::present() {
    GraphicsCommandList* commandList = currentCommandList2();

    //// End command list
    //detail::GraphicsCommandListInternal::endCommandRecoding(commandList);

    // Submit queue
    auto device = m_manager->deviceContext();
    detail::RHIResource* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(commandList, currentBackbuffer(), nullptr);
    device->queueSubmit(commandList->rhiResource(), rhiObject);
    detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(commandList);

    presentInternal();

    nextFrame();
}

void GraphicsContext::resetRHIBackbuffers() {
    uint32_t count = m_rhiObject->getBackbufferCount();
    m_backbuffers.resize(count);
    m_depthBuffers.resize(count);
    m_renderPasses.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        // backbuffer
        auto buffer = makeObject_deprecated<RenderTargetTexture>(this);
        detail::TextureInternal::resetRHIObject(buffer, m_rhiObject->getRenderTarget(i));
        m_backbuffers[i] = buffer;

        // DepthBuffer
        auto depthBuffer = makeObject_deprecated<DepthBuffer>(buffer->width(), buffer->height());
        m_depthBuffers[i] = depthBuffer;

        // RenderPass
        auto renderPass = makeObject_deprecated<RenderPass>(buffer, depthBuffer);
        renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0x00);
        m_renderPasses[i] = renderPass;
    }
    m_imageIndex = -1;
}

void GraphicsContext::nextFrame() {
    m_rhiObject->acquireNextImage(&m_imageIndex);

    GraphicsCommandList* commandList = currentCommandList2();
    commandList->reset();
    //detail::GraphicsCommandListInternal::beginCommandRecoding(commandList);
}

void GraphicsContext::presentInternal() {
    detail::GraphicsManager* manager = detail::GraphicsResourceInternal::manager(this);
    auto device = manager->deviceContext();

    detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);
    device->queuePresent(rhi);

    manager->frameBufferCache()->gcObjects();
    manager->renderPassCache()->collectGarbage();
}

detail::ISwapChain* GraphicsContext::resolveRHIObject(GraphicsCommandList* context, bool* outModified) const {
    *outModified = false;
    return m_rhiObject;
}

//==============================================================================
// SwapChainInternal

namespace detail {

void SwapChainInternal::setBackendBufferSize(GraphicsContext* swapChain, int width, int height) {
    LN_NOTIMPLEMENTED();
}

void SwapChainInternal::setOpenGLBackendFBO(GraphicsContext* swapChain, uint32_t id) {
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
