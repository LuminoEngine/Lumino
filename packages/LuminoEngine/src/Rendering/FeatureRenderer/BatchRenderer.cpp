#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchInstructionEncoder.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Mesh/detail/SpriteMeshGenerater.hpp>

namespace ln {
namespace detail {

class InstructionBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    BatchInstruction* first;
    BatchInstruction* last;

    InstructionBatchProxy()
        : material(nullptr)
        , first(nullptr)
        , last(nullptr) {}

    void getBatch(kanata::BatchCollector* collector) override {
        BatchInstructionEncoder* dispatcher = collector->manager()->batchInstructionDispatcher();
        dispatcher->dispatchList(collector, material, first);
    }
};

} // namespace detail

BatchRenderer* BatchRenderer::get() {
    return detail::RenderingManager::instance()->spriteRenderer();
}

BatchRenderer::BatchRenderer()
    : m_commandList(nullptr)
    , m_currentProxy(nullptr) {
}

Result_deprecated<> BatchRenderer::init() {
    if (!Object::init()) return err();
    return ok();
}

void BatchRenderer::begin(CommandList* commandList, Material* material) {
    if (LN_ASSERT(!m_commandList)) return;
    if (LN_ASSERT(commandList)) return;
    if (LN_ASSERT(material)) return;
    m_commandList = commandList;

    auto& collector = m_commandList->batchProxyCollector();
    m_currentProxy = collector->newSingleFrameBatchProxy<detail::InstructionBatchProxy>();
    m_currentProxy->material = material;
}

void BatchRenderer::end() {
    m_commandList = nullptr;
    m_currentProxy = nullptr;
}

void BatchRenderer::drawSprite(const SpriteData& sprite) {
    auto& collector = m_commandList->batchProxyCollector();
    detail::SpriteMeshGenerater* instruction = collector->newFrameRawData<detail::SpriteMeshGenerater>();
    instruction->type = detail::BatchInstructionType::StandardMesh;
    instruction->setTransform(sprite.transform);
    instruction->sprite = sprite;
    instruction->next = nullptr;
    if (!m_currentProxy->first) {
        m_currentProxy->first = instruction;
        m_currentProxy->last = instruction;
    }
    else {
        m_currentProxy->last->next = instruction;
        m_currentProxy->last = instruction;
    }
}

void BatchRenderer::drawSprite(
    const Matrix& transform,
    const Size& size,
    const Vector2& anchorRatio,
    const Rect& srcUVRect,
    const Color& color,
    SpriteBaseDirection baseDirection,
    BillboardType billboardType,
    const Flags<SpriteFlipFlags>& flipFlags) {
    drawSprite({
        transform,
        size,
        anchorRatio,
        srcUVRect,
        color,
        baseDirection,
        billboardType,
        flipFlags,
    });
}

} // namespace ln
