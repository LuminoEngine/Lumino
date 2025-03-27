#pragma once
#include <LuminoEngine/Graphics/Font/Common.hpp>
#include <LuminoEngine/Graphics/GPU/SamplerState.hpp>
#include "../Common.hpp"

namespace ln {

class SpriteTextRenderer : public Object {
    // NOTE: WPF のように FormattedText を使う案があったが廃止。
    // 単に文字を書きたいだけなのにオジェクトを作る必要があるのが面倒。またオーバーヘッドもある。
public:
    void begin(CommandList* commandList, Material* material);
    void end();
    void drawFillText(
        const detail::FormattedText* text,
        const Vector2& anchorRatio,
        SpriteBaseDirection baseDirection,
        const Ref<SamplerState>& samplerState,
        const Matrix& transform);
    void drawFillText(
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
        detail::FontRequester* fontRequester);
    void drawStrokeText(
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
        detail::FontRequester* fontRequester);

private:
    struct DrawTextEntry {
        Matrix transform;
        Vector2 anchorRatio;
        SpriteBaseDirection baseDirection;
        Ref<SamplerState> samplerState;
        float strokeWidth;

        // FormattedText
	    String text;
        Ref<Font> font; // TODO: obsolete
        Color color;
        Rect area;
        TextAlignment textAlignment;
        Ref<detail::FontRequester> fontRequester;
    };

    SpriteTextRenderer();
    ~SpriteTextRenderer() override;
    Result<> init();
    void flush();

    static const size_t MaxBatchSprites = 16383;

    CommandList* m_commandList;
    Material* m_material;
    std::vector<DrawTextEntry> m_entries;

    class DrawTextElementSFBatchProxy;
    friend class detail::RenderingManager;
};

} // namespace ln

