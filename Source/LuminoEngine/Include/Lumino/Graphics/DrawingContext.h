
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN

/**
	@brief	矩形、テキスト、イメージなどのオブジェクトを描画するための機能を提供します。
*/
class DrawingContext
	: public DrawList
{
public:

	/**
		@brief		指定したテクスチャを描画します。
		@param[in]	destRect	: 描画先の領域
		@param[in]	texture		: 描画するテクスチャ
		@param[in]	sourceRect	: 描画するテクスチャの転送領域
	*/
	void DrawTexture(const RectF& destRect, Texture* texture, const RectF& sourceRect);

	void DrawBoxBorder(
		const RectF& rect, const ThicknessF& thickness,
		const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor);

	void DrawBoxShadow(const RectF& rect, const Color& color, float blur, float width, bool inset);

LN_CONSTRUCT_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize();
};

LN_NAMESPACE_END
