
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/DepthBuffer.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include "RenderTargetTextureCache.hpp"
#include <LuminoGraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoGraphics/GPU/detail/RenderingCommandList.hpp>
#include "RenderPassCache.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
    : m_manager(nullptr)
    , m_rhiObject(nullptr)
    , m_backbuffers()
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

    nextFrame();
}

void SwapChain::onDispose(bool explicitDisposing) {
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

void SwapChain::onChangeDevice(detail::IGraphicsDevice* device) {
}

Size SwapChain::backbufferSize() const {
    const RenderTargetTexture* backbuffers = m_backbuffers[0];
    return Size(backbuffers->width(), backbuffers->height());
}

void SwapChain::resizeBackbuffer(int width, int height) {
    GraphicsCommandList* commandList = currentCommandList2();
    if (commandList) {
        LN_ASSERT(commandList->m_scopeState == GraphicsCommandList::ScopeState::Idle);
    }

    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
    resetRHIBackbuffers();
    nextFrame();
}

RenderTargetTexture* SwapChain::currentBackbuffer() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_backbuffers[m_imageIndex];
}

GraphicsCommandList* SwapChain::currentCommandList2() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_commandLists[m_imageIndex];
}

RenderPass* SwapChain::currentRenderPass() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_renderPasses[m_imageIndex];
}

void SwapChain::present() {
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

void SwapChain::resetRHIBackbuffers() {
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

void SwapChain::nextFrame() {
    m_rhiObject->acquireNextImage(&m_imageIndex);

    GraphicsCommandList* commandList = currentCommandList2();
    commandList->reset();
    //detail::GraphicsCommandListInternal::beginCommandRecoding(commandList);
}

void SwapChain::presentInternal() {
    detail::GraphicsManager* manager = detail::GraphicsResourceInternal::manager(this);
    auto device = manager->deviceContext();

    detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);
    device->queuePresent(rhi);

    manager->frameBufferCache()->gcObjects();
    manager->renderPassCache()->collectGarbage();
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
