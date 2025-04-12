#include "Internal.hpp"
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/DebugPrint.hpp>

namespace ln {

DebugPrint::DebugPrint(BatchRenderer* batchRenderer)
    : m_batchRenderer(batchRenderer)
    , m_fontTexture()
    , m_material() {
}

DebugPrint::~DebugPrint() {
}

MaybeResult DebugPrint::init() {
    // CopyScreen
    {
        static const unsigned char data[] = {
#include "Resource/text_16.png.inl"
        };
        Result<Ref<Texture2D>> texture = Texture2D::createFromImageFileData(data, LN_ARRAY_SIZE_OF(data));
        if (!texture) {
            return LN_TO_ERROR(texture);
        }
        m_fontTexture = texture.value();
    }

    m_material = Material::create(m_fontTexture);

    return LN_MAKE_SUCCESS();
}

void DebugPrint::dispose() {
    m_fontTexture = nullptr;
}

void DebugPrint::print(CommandList* commandList, const std::string_view& text) {
    const float paddingLeft = 8;
    const float paddingTop = 8;
    const float frameWidth = 16;
    const float frameHeight = 20;
    const int imageCols = 16;
    const float textureWidth = static_cast<float>(m_fontTexture->width());
    const float textureHeight = static_cast<float>(m_fontTexture->height());
    m_batchRenderer->begin(commandList, m_material);

    for (size_t i = 0; i < text.size(); i++) {
        char codePoint = text[i];
        if (codePoint < 0 || 127 < codePoint) {
            codePoint = 63; // ?
        }
        const float x = (codePoint % imageCols) * frameWidth;
        const float y = (codePoint / imageCols) * frameHeight;
        const Rect srcUVRect(
            x / textureWidth,
            y / textureHeight,
            frameWidth / textureWidth,
            frameHeight / textureHeight);
        Matrix pos = Matrix::makeTranslation(Vector3(paddingLeft + static_cast<float>(i) * frameWidth, paddingTop, 0));
        m_batchRenderer->drawSprite(
            pos,
            Size(frameWidth, frameHeight),
            Vector2::Zero,
            srcUVRect,
            Color::White,
            SpriteBaseDirection::Basic2D,
            BillboardType::None,
            SpriteFlipFlags::None);
    }

    m_batchRenderer->end();
}

} // namespace ln
