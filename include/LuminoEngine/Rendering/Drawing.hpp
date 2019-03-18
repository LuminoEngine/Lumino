
#pragma once
#include "Common.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class Texture;

enum class BrushWrapMode
{
	Stretch,		/**< イメージ全体を引き延ばす。*/
	Tile,			/**< イメージを並べて表示する。*/
};

enum class BrushImageDrawMode
{
	Image,
	BoxFrame,		/**< 3x3 のボーダーを描画する。*/
	BorderFrame,	/**< 3x3 のボーダーを描画する。中央は描画しない。*/
};

namespace detail {

struct BrushRawData
{
	Color						color;
	Ref<Texture>	texture;
	Rect						srcRect;
	BrushWrapMode				wrapMode;
	BrushImageDrawMode			imageDrawMode;
	Thickness					borderThickness;
};

} // namespace detail


} // namespace ln

