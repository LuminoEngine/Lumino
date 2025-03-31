#include "Internal.hpp"
#include <LuminoEngine/Graphics/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/SwapChain.hpp>
#include <LuminoEngine/Graphics/GPU/RenderPass.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderDescriptor.hpp>
#include "RenderTargetTextureCache.hpp"
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/Graphics/GPU/detail/RenderingCommandList.hpp>
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

bool GraphicsContext::init(PlatformWindow* window) {
    // TODO: onChangeDevice でバックバッファをアタッチ
    if (!Object::init()) {
        return false;
    }

    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

    m_rhiResourceRegistry = makeURef<detail::RHIGraphicsObjectRegistry>(m_manager->resourceRegistry());

    // Create SwapChain
    if (window) {
        SizeI backbufferSize;
        window->getFramebufferSize(&backbufferSize.width, &backbufferSize.height);
        m_rhiObject = rhiDevice()->createSwapChain(window, backbufferSize);
    }
    else {
        LN_LOG_INFO("Create a GraphicsContext with no SwapChain.");
    }

    m_singleFrameConstantBufferAllocatorPageManager = makeRef<detail::SingleFrameUniformBufferAllocatorPageManager>(
        this,
        0x200000); // 2MB

    resetRHIBackbuffers();

    // CommandList
    const int count = getBackbufferCount();
    m_commandLists.resize(count);
    for (int i = 0; i < count; i++) {
        auto commandList = Ref<GraphicsCommandList>(LN_NEW GraphicsCommandList(this), false);
        commandList->init(detail::GraphicsResourceInternal::manager(this));
        m_commandLists[i] = commandList;
    }

    nextFrame();
    return true;
}

void GraphicsContext::onCreateRHIObjects() {
    if (m_rhiObject) {
        const int count = getBackbufferCount();
        m_backbuffers.resize(count);
        m_depthBuffers.resize(count);
        m_renderPasses.resize(count);
        for (int i = 0; i < count; i++) {
            // backbuffer
            auto buffer = makeObject_deprecated<RenderTargetTexture>(this);
            buffer->resetRHIObject(this, m_rhiObject->getRenderTarget(i));
            m_backbuffers[i] = buffer;

            // DepthBuffer
            auto depthBuffer = makeObject_deprecated<DepthBuffer>(buffer->width(), buffer->height());
            m_depthBuffers[i] = depthBuffer;

            // RenderPass
            auto renderPass = makeObject_deprecated<RenderPass>(buffer, depthBuffer);
            renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0x00);
            m_renderPasses[i] = renderPass;
        }
    }
}

//// TODO: 統合時に純粋仮想関数にする
//detail::IGraphicsDevice* GraphicsContext::rhiDevice() const {
//    return m_manager->deviceContext();
//}

void GraphicsContext::onDispose(bool explicitDisposing) {
    //if (!m_commandLists.empty()) {
    //    // End command list
    //    detail::GraphicsCommandListInternal::endCommandRecoding(currentCommandList2());
    //}

    if (m_singleFrameConstantBufferAllocatorPageManager) {
        m_singleFrameConstantBufferAllocatorPageManager->clear();
        m_singleFrameConstantBufferAllocatorPageManager = nullptr;
    }

    m_rhiObject = nullptr;
    for (auto& x : m_commandLists)
        x->dispose();
    m_commandLists.clear();
    m_depthBuffers.clear();
    m_backbuffers.clear();

    if (m_rhiResourceRegistry) {
        m_rhiResourceRegistry->unregisterAllObjects();
        m_rhiResourceRegistry = nullptr;
    }

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void GraphicsContext::onChangeDevice(detail::IGraphicsDevice* device) {
}

Size GraphicsContext::backbufferSize() const {
    const RenderTargetTexture* backbuffers = m_backbuffers[0];
    return Size(backbuffers->width(), backbuffers->height());
}

int GraphicsContext::getBackbufferCount() const {
    if (m_rhiObject) {
        return m_rhiObject->getBackbufferCount();
    }
    else {
        return 1;
    }
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

DepthBuffer* GraphicsContext::currentDepthBuffer() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_depthBuffers[m_imageIndex];
}

GraphicsCommandList* GraphicsContext::currentCommandList2() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_commandLists[m_imageIndex];
}

RenderPass* GraphicsContext::currentRenderPass() const {
    if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_renderPasses[m_imageIndex];
}

detail::RHIDeviceObject* GraphicsContext::getRHIObject(IGraphicsObject* object) const {
    return m_rhiResourceRegistry->get(object);
}

void GraphicsContext::present() {
    GraphicsCommandList* commandList = currentCommandList2();

    //// End command list
    //detail::GraphicsCommandListInternal::endCommandRecoding(commandList);

    // Submit queue
    detail::IGraphicsDevice* device = rhiDevice();
    detail::RHIResource* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(commandList, currentBackbuffer(), nullptr);
    device->queueSubmit(commandList->rhiResource(), rhiObject);
    detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(commandList);

    presentInternal();

    nextFrame();
}

void GraphicsContext::resetRHIBackbuffers() {
    onCreateRHIObjects();
    m_imageIndex = -1;
}

void GraphicsContext::nextFrame() {
    if (m_rhiObject) {
        m_rhiObject->acquireNextImage(&m_imageIndex);
    }
    else {
        m_imageIndex = 0;
    }

    GraphicsCommandList* commandList = currentCommandList2();
    commandList->reset();
}

void GraphicsContext::presentInternal() {
    GraphicsManager* manager = detail::GraphicsResourceInternal::manager(this);
    detail::IGraphicsDevice* device = rhiDevice();

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
