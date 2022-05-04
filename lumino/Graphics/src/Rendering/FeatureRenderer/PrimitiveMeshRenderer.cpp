#include <LuminoGraphics/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoGraphics/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include "../../Mesh/MeshGeneraters/MeshGenerater.hpp"

namespace ln {
namespace detail {
class DrawMeshGeneraterSFBatchProxy : public kanata::SingleFrameBatchProxy {
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

void PrimitiveMeshRenderer::begin(RenderingContext* renderingContext, Material* material) {
    begin(renderingContext->commandList(), material);
}

void PrimitiveMeshRenderer::begin(CommandList* commandList, Material* material) {
    if (LN_ASSERT(!m_commandList)) return;
    if (LN_ASSERT(commandList)) return;
    if (LN_ASSERT(material)) return;
    m_commandList = commandList;
    m_material = material;
    m_generators.clear();
}

void PrimitiveMeshRenderer::end() {
    if (m_generators.length() > 0) {
        auto& collector = m_commandList->batchProxyCollector();

        const size_t size = sizeof(detail::MeshGenerater*) * m_generators.size();
        auto* generators = collector->dataAllocator()->allocate(size);
        memcpy(generators, m_generators.data(), size);

        auto* proxy = collector->newSingleFrameBatchProxy<detail::DrawMeshGeneraterSFBatchProxy>();
        proxy->material = m_material;
        proxy->generaters = static_cast<detail::MeshGenerater**>(generators);
        proxy->count = m_generators.length();
    }

    m_commandList = nullptr;
    m_material = nullptr;
}

void PrimitiveMeshRenderer::drawPlane(float width, float depth, const Color& color) {
    drawPlane(width, depth, Vector2::Zero, Vector2::Ones, color);
}

void PrimitiveMeshRenderer::drawPlane(float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color) {
    auto& collector = m_commandList->batchProxyCollector();
    auto* data = collector->newFrameRawData<detail::PlaneMeshGenerater>();
    data->size.set(width, depth);
    data->uv[0] = uv1;
    data->uv[1] = uv2;
    data->setColor(color);
    m_generators.push(data);
}

void PrimitiveMeshRenderer::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform) {
    auto& collector = m_commandList->batchProxyCollector();
    auto* data = collector->newFrameRawData<detail::RegularSphereMeshFactory>();
    data->m_radius = radius;
    data->m_slices = slices;
    data->m_stacks = stacks;
    data->setColor(color);
    data->setTransform(localTransform);
    m_generators.push(data);
}

void PrimitiveMeshRenderer::drawBox(const Box& box, const Color& color, const Matrix& localTransform) {
    auto& collector = m_commandList->batchProxyCollector();
    auto* data = collector->newFrameRawData<detail::RegularBoxMeshFactory>();
    data->m_size = Vector3(box.width, box.height, box.depth);
    data->setColor(color);
    data->setTransform(localTransform);
    m_generators.push(data);
}

} // namespace ln

