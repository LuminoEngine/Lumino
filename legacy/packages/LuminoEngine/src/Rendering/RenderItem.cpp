#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/RendererServer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/RenderItem.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>

namespace ln {
namespace detail {

MeshDrawElement::MeshDrawElement()
    : DrawElement(SceneRenderPass::DrawElementType::Sprite)
    , m_batchProxyState(LN_NEW kanata::BatchProxyState()) {
    singleFrameResource = false;
    m_batchProxyState->reset();
}

MeshDrawElement::~MeshDrawElement() {
}

void MeshDrawElement::onRender(RendererServer* rendererServer, CommandList* commandList) {
    kanata::BatchCollector* collector = commandList->batchCollector();
    collector->batchProxyState = m_batchProxyState.get();

    if (1) {
        const int surfaceCount = mesh->m_surfaces.size();
        for (int iSurface = 0; iSurface < surfaceCount; iSurface++) {
            const MeshSurfaceData& surface = mesh->m_surfaces[iSurface];
            auto* renderer = rendererServer->batchRenderer();
            rendererServer->activate(renderer, commandList, surface.material);
            renderer->drawMesh(worldTransform, mesh, iSurface);
        }
    }
    else {

        const int surfaceCount = mesh->m_surfaces.size();
        for (int iSurface = 0; iSurface < surfaceCount; iSurface++) {
            const MeshSurfaceData& surface = mesh->m_surfaces[iSurface];
            // Make batch
            kanata::Batch* batch = collector->newBatch<kanata::Batch>(1, surface.material);

            batch->elemets2[0].vertexBuffers[0] = surface.mainVertexBuffer;
            batch->elemets2[0].indexBuffer = surface.indexBuffer;
            batch->elemets2[0].firstIndex = 0;
            batch->elemets2[0].primitiveCount = surface.indexCount / 3; // TODO: primitiveType による
            batch->vertexLayout = surface.vertexLayout;
            batch->primitiveTopology = PrimitiveTopology::TriangleList; //surface.topology;

            batch->worldTransform = worldTransform;
        }
    }



#if 0
        //const URef<kanata::BatchCollector>& batchCollector() const { return m_batchCollector; }
        Batch* batch = commandList->batchCollector()->newBatch();

        for (int i = 0; i < vbCount; ++i) {
            batch->elemets2[0].vertexBuffers[i] = vb[i];
        }
        batch->elemets2[0].indexBuffer = ib;
        batch->elemets2[0].firstIndex = section.startIndex;
        batch->elemets2[0].primitiveCount = section.primitiveCount;
        batch->vertexLayout = layout;
        batch->primitiveTopology = section.topology;
        batch->skeleton = skeleton;

        if (transform) {
            batch->worldTransform = Matrix::multiply(batch->worldTransform, *transform);
        }
        // MeshRenderFeature::drawMesh
#endif
}

} // namespace detail

Ref<RenderItem> RenderItem::create() {
    return Ref<RenderItem>(LN_NEW RenderItem(), false);
}

RenderItem::RenderItem()
    : m_drawElement(LN_NEW detail::MeshDrawElement()) {
}

void RenderItem::setMesh(Mesh* mesh) {
    m_drawElement->mesh = mesh;
}

void RenderItem::setTransform(const Matrix& matrix) {
    m_drawElement->worldTransform = matrix;
}

} // namespace ln
