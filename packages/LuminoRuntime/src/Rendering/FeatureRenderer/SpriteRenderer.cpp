#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KSpriteRenderFeature.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace detail {
class DrawSpriteSFBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    const SpriteData* sprites;
    int32_t count;
    void getBatch(kanata::BatchCollector* collector) override {
        auto* r = detail::RenderingManager::instance()->spriteRenderFeature().get();
        r->drawSpritesDirect(collector, material, sprites, count);
    }
};
} // namespace detail

SpriteRenderer* SpriteRenderer::get() {
    return detail::RenderingManager::instance()->spriteRenderer();
}

SpriteRenderer::SpriteRenderer()
    : m_commandList()
    , m_material(nullptr)
    , m_sprites() {
    m_sprites.reserve(MaxBatchSprites);
}

Result<> SpriteRenderer::init() {
    if (!Object::init()) return err();
    return ok();
}

void SpriteRenderer::begin(RenderingContext* renderingContext, Material* material) {
    begin(renderingContext->commandList(), material);
}

void SpriteRenderer::begin(CommandList* commandList, Material* material) {
    if (LN_ASSERT(!m_commandList)) return;
    if (LN_ASSERT(commandList)) return;
    if (LN_ASSERT(material)) return;
    m_commandList = commandList;
    m_material = material;
}

void SpriteRenderer::end() {
    if (m_sprites.size() > 0) {
        flush();
    }
    m_commandList = nullptr;
    m_material = nullptr;
}

void SpriteRenderer::drawSprite(const SpriteData& sprite) {
    if (m_sprites.size() >= MaxBatchSprites) {
        flush();
    }

    m_sprites.push_back(sprite);
}

void SpriteRenderer::drawSprite(
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

void SpriteRenderer::flush() {
    LN_DCHECK(m_sprites.size() > 0);

    auto& collector = m_commandList->batchProxyCollector();

    const size_t size = sizeof(SpriteData) * m_sprites.size();
    auto* sprites = collector->dataAllocator()->allocate(size);
    memcpy(sprites, m_sprites.data(), size);

    auto* proxy = collector->newSingleFrameBatchProxy<detail::DrawSpriteSFBatchProxy>();
    proxy->material = m_material;
    proxy->sprites = static_cast<const SpriteData*>(sprites);
    proxy->count = m_sprites.size();

    m_sprites.clear();
}

} // namespace ln

