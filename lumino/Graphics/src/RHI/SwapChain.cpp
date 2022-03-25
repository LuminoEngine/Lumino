
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/DepthBuffer.hpp>
#include <LuminoGraphics/RHI/SwapChain.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include "GraphicsManager.hpp"
#include "RenderTargetTextureCache.hpp"
#include "Backend/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/RHI/detail/RenderingCommandList.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
    : m_manager(nullptr)
    , m_rhiObject(nullptr)
    , m_backbuffers()
    , m_currentCommandList(nullptr)
    , m_imageIndex(-1) {
}

SwapChain::~SwapChain() {
}

void SwapChain::init(PlatformWindow* window) {
    // TODO: onChangeDevice でバックバッファをアタッチ
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

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
}

void SwapChain::onDispose(bool explicitDisposing) {
    m_rhiObject = nullptr;
    for (auto& x : m_commandLists)
        x->dispose();
    m_commandLists.clear();
    m_depthBuffers.clear();
    m_backbuffers.clear();

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void SwapChain::onChangeDevice(detail::IGraphicsDevice* device) {
}

Size SwapChain::backbufferSize() const {
    const RenderTargetTexture* backbuffers = m_backbuffers[0];
    return Size(backbuffers->width(), backbuffers->height());
}

RenderTargetTexture* SwapChain::currentBackbuffer() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_backbuffers[m_imageIndex];
}

void SwapChain::resizeBackbuffer(int width, int height) {
    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
    resetRHIBackbuffers();
}

GraphicsCommandList* SwapChain::beginFrame2() {
    m_rhiObject->acquireNextImage(&m_imageIndex);

    m_currentCommandList = currentCommandList();
    m_currentCommandList->reset();
    //detail::GraphicsCommandListInternal::resetCommandList(m_graphicsContext, rhiCommandList);
    detail::GraphicsCommandListInternal::beginCommandRecoding(m_currentCommandList);
    currentCommandList()->m_singleFrameUniformBufferAllocator->cleanup();
    m_currentCommandList->resetState();

    return m_currentCommandList;
}

RenderPass* SwapChain::currentRenderPass() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_renderPasses[m_imageIndex];
}

void SwapChain::endFrame() {
    currentCommandList()->m_singleFrameUniformBufferAllocator->unmap();
    // detail::GraphicsContextInternal::flushCommandRecoding(m_graphicsContext, currentBackbuffer());
    detail::GraphicsCommandListInternal::endCommandRecoding(m_currentCommandList);

    auto device = m_manager->deviceContext();
    detail::RHIResource* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(m_currentCommandList, currentBackbuffer(), nullptr);
    device->submitCommandBuffer(m_currentCommandList->rhiResource(), rhiObject);

    detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(m_currentCommandList);
    //detail::GraphicsContextInternal::resetCommandList(m_graphicsContext, nullptr);

    present(m_currentCommandList);

    m_currentCommandList = nullptr;
}

void SwapChain::resetRHIBackbuffers() {
    uint32_t count = m_rhiObject->getBackbufferCount();
    m_backbuffers.resize(count);
    m_depthBuffers.resize(count);
    m_renderPasses.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        // backbuffer
        auto buffer = makeObject<RenderTargetTexture>(this);
        detail::TextureInternal::resetRHIObject(buffer, m_rhiObject->getRenderTarget(i));
        m_backbuffers[i] = buffer;

        // DepthBuffer
        auto depthBuffer = makeObject<DepthBuffer>(buffer->width(), buffer->height());
        m_depthBuffers[i] = depthBuffer;

        // RenderPass
        auto renderPass = makeObject<RenderPass>(buffer, depthBuffer);
        renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0x00);
        m_renderPasses[i] = renderPass;
    }
    m_imageIndex = -1;
}

void SwapChain::present(GraphicsCommandList* context) {
    detail::GraphicsManager* manager = detail::GraphicsResourceInternal::manager(this);
    auto device = manager->deviceContext();

    detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);
    rhi->present();

    manager->frameBufferCache()->gcObjects();
}

detail::ISwapChain* SwapChain::resolveRHIObject(GraphicsCommandList* context, bool* outModified) const {
    *outModified = false;
    return m_rhiObject;
}

//==============================================================================
// SwapChainInternal

namespace detail {

void SwapChainInternal::setBackendBufferSize(SwapChain* swapChain, int width, int height) {
    LN_NOTIMPLEMENTED();
}

void SwapChainInternal::setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id) {
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
