#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/IndexBuffer.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KScreenRectangleRenderFeature.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KMeshRenderFeature.hpp>
#include "../RenderingManager.hpp"

namespace ln {
namespace kanata {

BatchCollector::BatchCollector(detail::RenderingManager* manager)
    : m_manager(manager)
    , m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
    , m_headSingleFrameBatchProxy(nullptr)
    , m_tailSingleFrameBatchProxy(nullptr)
    , m_headFrameData(nullptr)
    , m_tailFrameData(nullptr)
    , m_resolvedSingleFrameBatchProxies(false) {
    m_vertexBufferAllocatorManager = makeURef<detail::StreamingBufferAllocatorManager>(detail::StreamingBufferPage::Type::VertexBuffer, sizeof(Vertex));
    m_indexufferAllocatorManager = makeURef<detail::StreamingBufferAllocatorManager>(detail::StreamingBufferPage::Type::IndexBuffer, sizeof(uint32_t));
    m_vertexBufferAllocator = makeURef<detail::StreamingBufferAllocator>(m_vertexBufferAllocatorManager);
    m_indexBufferAllocator = makeURef<detail::StreamingBufferAllocator>(m_indexufferAllocatorManager);
    m_screenRectangleRenderFeature = makeURef<ScreenRectangleRenderFeature>(manager);
    m_screenRectangleRenderFeature->init();
    m_primitiveRenderer = makeURef<PrimitiveMeshRenderer>(manager);
    m_meshRenderFeature = makeURef<MeshRenderFeature>(manager);
}

BatchCollector::~BatchCollector() {

}

VertexLayout* BatchCollector::standardVertexDeclaration() const {
    return m_manager->standardVertexDeclaration();
}

void BatchCollector::clear() {
    // destruct single frame data.
    {
        IBatchFrameData* p = m_headFrameData;
        while (p) {
            p->~IBatchFrameData();
            p = p->m_nextFrameData;
        }
        m_headFrameData = nullptr;
        m_tailFrameData = nullptr;
    }
    {
        SingleFrameBatchProxy* p = m_headSingleFrameBatchProxy;
        while (p) {
            p->~SingleFrameBatchProxy();
            p = p->m_next;
        }
        m_headSingleFrameBatchProxy = nullptr;
        m_tailSingleFrameBatchProxy = nullptr;
    }

    m_dataAllocator->cleanup();
    m_vertexBufferAllocator->cleanup();
    m_indexBufferAllocator->cleanup();
    m_batchList.clear();
    m_resolvedSingleFrameBatchProxies = false;
    m_currentRenderPass = nullptr;
}

void BatchCollector::setRenderPass(RenderPass* value) {
    m_currentRenderPass = value;
}

void BatchCollector::resolveSingleFrameBatchProxies() {
    LN_CHECK(!m_resolvedSingleFrameBatchProxies);
    SingleFrameBatchProxy* p = m_headSingleFrameBatchProxy;
    while (p) {
        p->getBatch(this);
        p = p->m_next;
    }
    m_resolvedSingleFrameBatchProxies = true;
}

void BatchCollector::addBatch(Batch* batch) {
    batch->renderPass = m_currentRenderPass;
    m_batchList.push(batch);
}

MeshBufferView BatchCollector::allocateMeshBuffer(int32_t vertexCount, int32_t indexCount) {
    const auto view1 = m_vertexBufferAllocator->allocate(vertexCount);
    const auto view2 = m_indexBufferAllocator->allocate(indexCount);

    MeshBufferView view;
    view.vertexBuffer = static_cast<VertexBuffer*>(view1.resource);
    view.indexBuffer = static_cast<IndexBuffer*>(view2.resource);
    view.vertexData = reinterpret_cast<Vertex*>(view.vertexBuffer->writableData());
    view.indexData = reinterpret_cast<int32_t*>(view.indexBuffer->map(MapMode::Write));
    view.firstIndex = view2.offset / sizeof(int32_t);
    view.vertexOffset = view1.offset / sizeof(Vertex);
    return view;
}

void BatchCollector::addSingleFrameBatchProxy(SingleFrameBatchProxy* batchProxy) {
    LN_CHECK(batchProxy);
    if (!m_headSingleFrameBatchProxy) {
        m_headSingleFrameBatchProxy = batchProxy;
    }
    else {
        m_tailSingleFrameBatchProxy->m_next = batchProxy;
    }
    m_tailSingleFrameBatchProxy = batchProxy;
}

void BatchCollector::addFrameData(IBatchFrameData* data) {
    LN_CHECK(data);
    if (!m_headFrameData) {
        m_headFrameData = data;
    }
    else {
        m_tailFrameData->m_nextFrameData = data;
    }
    m_tailFrameData = data;
}

} // namespace kanata
} // namespace ln

