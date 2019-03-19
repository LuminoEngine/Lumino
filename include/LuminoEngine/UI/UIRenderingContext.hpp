#pragma once
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "../Rendering/Drawing.hpp"

namespace ln {

// 従来の Brush 相当の塗りつぶしパラメータは Material。形状にかかわるものはそれ以外に分離(strokeSize など)
// ただし、グラデーションなども Brush 相当のパラメータとは分離して考える。Web Canvas。
//
// 一般的なペイントシステムと大きく異なる点は、通常 Image を使うような箇所に Material を使うこと。
class UIRenderingContext
	: public RenderingContext
{
public:
    UIRenderingContext();

    void drawBoxBackground(const Rect& rect, const Thickness& borderThickness, const CornerRadius& cornerRadius, BrushImageDrawMode mode, AbstractMaterial* material, const Rect& textureSourceRect);

public: // TODO: inernal
	void resetForBeginRendering();

	Ref<detail::DrawElementList> m_elementList;
};

} // namespace ln

