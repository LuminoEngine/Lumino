
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN

namespace detail {

struct BoxBorderData
{
	ThicknessF		borderThickness;
	CornerRadius	cornerRadius;
	Color			leftBorderColor;
	Color			topBorderColor;
	Color			rightBorderColor;
	Color			bottomBorderColor;
	BorderDirection	borderDirection;
};

struct BoxShadowData
{
	Color			shadowColor;
	float			shadowBlur;
	float			shadowWidth;
	ShadowDirection	shadowDirection;
};

} // namespace detail

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
	void DrawTexture(const Rect& destRect, Texture* texture, const Rect& sourceRect);


	void DrawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius);

	void DrawBoxBorder(
		const Rect& rect, const ThicknessF& thickness, const CornerRadius& cornerRadius,
		const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
		BorderDirection borderDirection);

	void DrawBoxBorder(
		const Rect& rect, const ThicknessF& thickness,
		const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
		float ltRad, float rtRad, float lbRad, float rbRad, BorderDirection borderDirection,
		const Color& shadowColor, float shadowBlur, float shadowWidth, ShadowDirection shadowDirection);

	void DrawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, ShadowDirection shadowDirection);

LN_CONSTRUCT_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void initialize();
};

LN_NAMESPACE_END
