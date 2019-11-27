﻿#pragma once
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "../Rendering/Drawing.hpp"

namespace ln {
class UIElement;

// 従来の Brush 相当の塗りつぶしパラメータは Material。形状にかかわるものはそれ以外に分離(strokeSize など)
// ただし、グラデーションなども Brush 相当のパラメータとは分離して考える。Web Canvas。
//
// 一般的なペイントシステムと大きく異なる点は、通常 Image を使うような箇所に Material を使うこと。
class UIRenderingContext
	: public RenderingContext
{
public:
    UIRenderingContext();

    // 単色塗りつぶし
    void drawSolidRectangle(const Rect& rect, const Color& color);

    void drawImageBox(const Rect& rect, BrushImageDrawMode mode, const Rect& textureSourceRect, const Thickness& borderThickness, const Color& color);


    //void drawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius, BrushImageDrawMode mode/*, AbstractMaterial* material*/, const Rect& textureSourceRect, const Color& color);

    void drawBoxBorderLine(const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset);
	void drawBoxBorderLine(const Rect& rect, float thickness, const Color& color, bool borderInset);

	void drawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Vector2& offset, const Color& color, float blur, float width, bool inset);

    void drawImage(const Rect& destinationRect, AbstractMaterial* material);

	void drawVisual(UIElement* element, const Matrix& transform);

public: // TODO: inernal
	void resetForBeginRendering();

	Ref<detail::DrawElementList> m_elementList;
    bool m_adornerRendering = false;
};

} // namespace ln

