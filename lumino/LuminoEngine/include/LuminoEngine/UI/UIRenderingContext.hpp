#pragma once
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/Drawing.hpp>

namespace ln {
class UIElement;
class UITheme;

// 従来の Brush 相当の塗りつぶしパラメータは Material。形状にかかわるものはそれ以外に分離(strokeSize など)
// ただし、グラデーションなども Brush 相当のパラメータとは分離して考える。Web Canvas。
//
// 一般的なペイントシステムと大きく異なる点は、通常 Image を使うような箇所に Material を使うこと。
class UIRenderingContext
	: public RenderingContext
{
    /*
	RenderingContext と UIRenderingContext は分けるべきか？
	----------
	機能的にはまとめしまってもよい。でも、UI と Scene の境界で分けるようにしたい。
	ただ、RenderView で分ける、というよりは、エディタコンテンツか、ランタイムコンテンツか、で分けたい気持ち。
	例えばフレームデバッガを使うときは、ランタイムコンテンツだけに対して使いたい。
	なので、最低限、次の2つで分けることにしてみたい。
	- UIFrameWindow 
	- Scene と、Scene 内の UI
	*/
public:
    UIRenderingContext();

    // 単色塗りつぶし
    void drawSolidRectangle(const Rect& rect, const Color& color);

    void drawImageBox(const Rect& rect, Sprite9DrawMode mode, const Rect& textureSourceRect, const Thickness& borderThickness, const Color& color);


    //void drawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius, BrushImageDrawMode mode/*, Material* material*/, const Rect& textureSourceRect, const Color& color);

    void drawBoxBorderLine(const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset);
	void drawBoxBorderLine(const Rect& rect, float thickness, const Color& color, bool borderInset);

	void drawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Vector2& offset, const Color& color, float blur, float width, bool inset);

    void drawBoxElement(const BoxElementShapeBaseStyle& baseStyle, const BoxElementShapeBackgroundStyle* backbroundStyle = nullptr, const BoxElementShapeBorderStyle* borderStyle = nullptr, const BoxElementShapeShadowStyle* shadowStyle = nullptr);

    void drawImage(const Rect& destinationRect, Material* material);

	void drawVisual(UIElement* element, const Matrix& transform);

	UITheme* theme() const;


	//void pushScissorRect(const RectI& rect);
	//void popScissorRect();

public: // TODO: inernal
	void resetForBeginRendering(const RenderViewPoint* value);

    bool m_adornerRendering = false;
	Ref<UITheme> m_theme;

	// onRender の中で、blit の位置調整のために使いたい
	//Rect m_currentRenderTargetRect;

	//std::stack<RectI> m_scissorStack;
};

} // namespace ln

