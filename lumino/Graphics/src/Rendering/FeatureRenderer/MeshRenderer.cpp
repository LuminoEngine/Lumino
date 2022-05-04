#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/Kanata/RenderFeature/KMeshRenderFeature.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/MeshRenderer.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>

namespace ln {

    
#if 0
namespace detail {

class DrawMeshSFBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    MeshGenerater** generaters;
    int32_t count;
    void getBatch(kanata::BatchCollector* collector) override {
        auto* r = detail::RenderingManager::instance()->primitiveRenderer().get();
        r->drawGeneratorsDirect(collector, material, generaters, count);
    }
};
} // namespace detail

PrimitiveMeshRenderer* PrimitiveMeshRenderer::get() {
    return detail::RenderingManager::instance()->primitiveMeshRenderer();
}

PrimitiveMeshRenderer::PrimitiveMeshRenderer()
    : m_commandList(nullptr)
    , m_material(nullptr)
    , m_generators() {
}

Result PrimitiveMeshRenderer::init() {
    if (!Object::init()) return err();
    return ok();
}


void PrimitiveMeshRenderer::drawPlane(float width, float depth, const Color& color) {
    drawPlane(width, depth, Vector2::Zero, Vector2::Ones, color);
}
#endif

MeshBatchProxy::MeshBatchProxy() {
}

MeshBatchProxy::~MeshBatchProxy() {
}

void MeshBatchProxy::getBatch(kanata::BatchCollector* collector) {
    kanata::MeshRenderFeature* renderer = detail::RenderingManager::instance()->meshRenderFeature();
    MeshModel* model = modelInstance->model();
    //->drawMesh(collector, material, mesh, sectionIndex);

    for (const auto& node : model->meshNodes()) {
        if (node->meshContainerIndex() >= 0) {
            const Matrix& transform = model->nodeGlobalTransform(node->index());

            const auto& meshContainer = model->meshContainers()[node->meshContainerIndex()];
            for (const auto& meshPrimitive : meshContainer->meshPrimitives()) {

                for (int iSection = 0; iSection < meshPrimitive->sections().size(); iSection++) {
                    int materialIndex = meshPrimitive->sections()[iSection].materialIndex;
                    if (materialIndex >= 0) {
                        Material* material = modelInstance->getActualMaterial(materialIndex);
                        renderer->drawMesh(
                            collector,
                            material,
                            meshPrimitive,
                            iSection,
                            &transform, 
                            modelInstance->hasSkeleton() ? modelInstance->skeletons()[node->skeletonIndex] : nullptr,
                            modelInstance->hasMorph() ? modelInstance->morphs()[node->meshContainerIndex()] : nullptr);
                    }
                }
            }
        }
    }
}

} // namespace ln
