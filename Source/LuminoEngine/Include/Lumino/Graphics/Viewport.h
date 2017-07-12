
#pragma once
#include "../BindingSupport.h"
#include "Texture.h"
#include "Color.h"
#include "ImageEffect/ImageEffect.h"

LN_NAMESPACE_BEGIN

/** Viewport の配置方法 */
enum class ViewportPlacement
{
	Stretch,		/**< 転送先領域全体に拡大または縮小する */
	AutoResize,		/**< 転送先領域と同じピクセルサイズになるよう自動的にリサイズする */
	ViewBox,		/**< 転送元領域のアスペクト比が維持されるように余白を挿入する */
};

LN_NAMESPACE_END
