
#pragma once
#include "Common.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class Texture;

enum class Sprite9DrawMode
{
	/** ボーダーを描画しない。(普通のスプライトと同様) */
	StretchedSingleImage,

	/** ボーダーを描画しない。(普通のスプライトと同様) */
	RepeatedSingleImage,

	/** 3x3 のボーダーを描画する。中央および上下左右のスプライトはイメージを拡大して描画する。 */
	StretchedBoxFrame,

	/** 3x3 のボーダーを描画する。中央および上下左右のスプライトはイメージを繰り返して描画する。 */
	RepeatedBoxFrame,

	/** 3x3 のボーダーを描画する。中央は描画しない。上下左右のスプライトはイメージを拡大して描画する。 */
	StretchedBorderFrame,

	/** 3x3 のボーダーを描画する。中央は描画しない。上下左右のスプライトはイメージを繰り返して描画する。 */
	RepeatedBorderFrame,
};

//enum class BrushImageDrawMode
//{
//	Image,
//	BoxFrame,		
//	BorderFrame,	/**< 3x3 のボーダーを描画する。中央は描画しない。*/
//};

//namespace detail {
//
//struct BrushRawData
//{
//	Color						color;
//	Ref<Texture>	texture;
//	Rect						srcRect;
//	BrushWrapMode				wrapMode;
//	BrushImageDrawMode			imageDrawMode;
//	Thickness					borderThickness;
//};
//
//} // namespace detail


} // namespace ln

