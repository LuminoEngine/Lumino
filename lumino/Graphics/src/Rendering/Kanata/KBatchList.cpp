#include <LuminoGraphics/GPU/VertexLayout.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/IndexBuffer.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatch.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchList.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

BatchCollector::BatchCollector(detail::RenderingManager* manager)
    : batchProxyState(nullptr)
    , m_manager(manager)
    , m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
    , m_headFrameData(nullptr)
    , m_tailFrameData(nullptr)
    , m_viewPoint(nullptr) {
    m_vertexBufferAllocatorManager = makeURef<detail::StreamingBufferAllocatorManager>(detail::StreamingBufferPage::Type::VertexBuffer, sizeof(Vertex));
    m_indexufferAllocatorManager = makeURef<detail::StreamingBufferAllocatorManager>(detail::StreamingBufferPage::Type::IndexBuffer, sizeof(uint32_t));
    m_vertexBufferAllocator = makeURef<detail::StreamingBufferAllocator>(m_vertexBufferAllocatorManager);
    m_indexBufferAllocator = makeURef<detail::StreamingBufferAllocator>(m_indexufferAllocatorManager);
}

BatchCollector::~BatchCollector() {
    LN_DCHECK(!m_headFrameData); // Not calling dispose
}

void BatchCollector::dispose() {
    clear(nullptr);
}

VertexLayout* BatchCollector::standardVertexDeclaration() const {
    return m_manager->standardVertexDeclaration();
}

void BatchCollector::clear(const RenderViewPoint* viewPoint) {
    m_viewPoint = viewPoint;

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

    m_dataAllocator->cleanup();
    m_vertexBufferAllocator->cleanup();
    m_indexBufferAllocator->cleanup();
    m_batchList.clear();
    m_currentRenderPass = nullptr;
}

void BatchCollector::setRenderPass(RenderPass* value) {
    m_currentRenderPass = value;
}


void BatchCollector::addBatch(Batch* batch) {
    batch->renderPass = m_currentRenderPass;
    m_batchList.push(batch);
}

MeshBufferView BatchCollector::allocateMeshBuffer(int32_t vertexCount, int32_t indexCount) {
    const auto view1 = m_vertexBufferAllocator->allocate(vertexCount);
    const auto view2 = m_indexBufferAllocator->allocate(indexCount);
    if (view1.elementCount != vertexCount) {
        LN_NOTIMPLEMENTED();    // TODO: 本来は呼び出し側で気を付けるべき
    }
    if (view2.elementCount != indexCount) {
        LN_NOTIMPLEMENTED(); // TODO: 本来は呼び出し側で気を付けるべき
    }

    MeshBufferView view;
    view.vertexBuffer = static_cast<VertexBuffer*>(view1.resource);
    view.indexBuffer = static_cast<IndexBuffer*>(view2.resource);
    view.vertexData = reinterpret_cast<Vertex*>(view.vertexBuffer->writableData()) + view1.elementOffsets;
    view.indexData = reinterpret_cast<uint32_t*>(view.indexBuffer->map(MapMode::Write)) + view2.elementOffsets;
    view.firstIndex = view2.elementOffsets;
    view.vertexOffset = view1.elementOffsets;
    view.vertexCount = view1.elementCount;
    view.indexCount = view2.elementCount;
    return view;
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

void BatchCollector::resolveBatchMaterial(Batch* batch, Material* material) {
    LN_DCHECK(material);
    LN_DCHECK(batchProxyState);
    batch->material.material = material;
    batch->material.blendMode = material->getBlendMode().valueOr(batchProxyState->m_blendMode.valueOr(BlendMode::Normal));
    batch->material.cullingMode = material->getCullingMode().valueOr(batchProxyState->m_cullingMode.valueOr(CullMode::Back));
    batch->material.depthTestEnabled = material->isDepthTestEnabled().valueOr(batchProxyState->m_depthTestEnabled.valueOr(true));
    batch->material.depthWriteEnabled = material->isDepthWriteEnabled().valueOr(batchProxyState->m_depthWriteEnabled.valueOr(true));
    batch->material.shadingModel = batchProxyState->shadingModel.valueOr(material->shadingModel());

    if (batchProxyState->builtinEffectData) {
        batch->builtinEffectData = *batchProxyState->builtinEffectData;
    }

    if (batchProxyState->baseTransform) {
        batch->worldTransform = Matrix::multiply(*batchProxyState->baseTransform, batchProxyState->transform);
    }
    else {
        batch->worldTransform = batchProxyState->transform;
    }
}

} // namespace kanata
} // namespace ln

