#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KSpriteTextRenderFeature.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteTextRenderer.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingManager.hpp>

namespace ln {
class SpriteTextRenderer::DrawTextElementSFBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    const DrawTextEntry* entries;
    int32_t count;

    void getBatch(kanata::BatchCollector* collector) override {
        kanata::SpriteTextRenderFeature* r = detail::RenderingManager::instance()->spriteTextRenderFeature();
        r->beginBatch(collector, material);
        for (int32_t i = 0; i < count; i++) {
			const auto& entry = entries[i];
            r->drawText(
                entry.transform,
                entry.anchorRatio,
                entry.baseDirection,
                entry.samplerState,
                entry.strokeWidth,
                entry.text,
                entry.font,
                entry.color,
                entry.area,
                entry.textAlignment,
                entry.fontRequester
            );
		}
        r->endBatch(collector);
    }
};

SpriteTextRenderer::SpriteTextRenderer()
    : m_commandList()
    , m_material(nullptr)
    , m_entries() {
    m_entries.reserve(MaxBatchSprites);
}

SpriteTextRenderer::~SpriteTextRenderer() {
}

Result_deprecated<> SpriteTextRenderer::init() {
    if (!Object::init()) return err();
    return ok();
}

void SpriteTextRenderer::begin(CommandList* commandList, Material* material) {
    if (LN_ASSERT(!m_commandList)) return;
    if (LN_ASSERT(commandList)) return;
    if (LN_ASSERT(material)) return;
    m_commandList = commandList;
    m_material = material;
}

void SpriteTextRenderer::end() {
    if (m_entries.size() > 0) {
        flush();
    }
    m_commandList = nullptr;
    m_material = nullptr;
}

void SpriteTextRenderer::drawFillText(
    const detail::FormattedText* text,
    const Vector2& anchorRatio,
    SpriteBaseDirection baseDirection,
    const Ref<SamplerState>& samplerState,
    const Matrix& transform) {
    drawFillText(
        transform,
        anchorRatio,
        baseDirection,
        samplerState,
        text->text,
        text->font,
        text->color,
        text->area,
        text->textAlignment,
        text->fontRequester
    );
}

void SpriteTextRenderer::drawFillText(
    const Matrix& transform,
    const Vector2& anchorRatio,
    SpriteBaseDirection baseDirection,
    SamplerState* samplerState,
    // FormattedText
    const StringView& text,
    Font* font, // TODO: obsolete
    const Color& color,
    const Rect& area,
    TextAlignment textAlignment,
    detail::FontRequester* fontRequester) {

    if (m_entries.size() >= MaxBatchSprites) {
        flush();
    }

    DrawTextEntry entry;
    entry.strokeWidth = 0;
    entry.transform = transform;
    entry.anchorRatio = anchorRatio;
    entry.baseDirection = baseDirection;
    entry.samplerState = samplerState;

    entry.text = text;
    entry.font = font;
    entry.color = color;
    entry.area = area;
    entry.textAlignment = textAlignment;
    entry.fontRequester = fontRequester;

    m_entries.push_back(entry);
}

void SpriteTextRenderer::drawStrokeText(
    float strokeWidth,
    const Matrix& transform,
    const Vector2& anchorRatio,
    SpriteBaseDirection baseDirection,
    SamplerState* samplerState,
    // FormattedText
    const StringView& text,
    Font* font, // TODO: obsolete
    const Color& color,
    const Rect& area,
    TextAlignment textAlignment,
    detail::FontRequester* fontRequester) {

    if (m_entries.size() >= MaxBatchSprites) {
        flush();
    }

    DrawTextEntry entry;
    entry.strokeWidth = strokeWidth;
    entry.transform = transform;
    entry.anchorRatio = anchorRatio;
    entry.baseDirection = baseDirection;
    entry.samplerState = samplerState;

    entry.text = text;
    entry.font = font;
    entry.color = color;
    entry.area = area;
    entry.textAlignment = textAlignment;
    entry.fontRequester = fontRequester;

    m_entries.push_back(entry);
}

void SpriteTextRenderer::flush() {
    LN_DCHECK(m_entries.size() > 0);

    auto& collector = m_commandList->batchProxyCollector();

    const size_t size = sizeof(DrawTextEntry) * m_entries.size();
    DrawTextEntry* entries = static_cast<DrawTextEntry*>(collector->dataAllocator()->allocate(size));
    // Ref<> 使ってるので memcpy ではなくちゃんとコピーする。
    for (int i = 0; i < m_entries.size(); i++) {
        new (entries + i) DrawTextEntry(std::move(m_entries[i]));
    }

    auto* proxy = collector->newSingleFrameBatchProxy<DrawTextElementSFBatchProxy>();
    proxy->material = m_material;
    proxy->entries = entries;
    proxy->count = m_entries.size();

    m_entries.clear();
}


} // namespace ln

