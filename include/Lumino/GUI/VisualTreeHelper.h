
#pragma once
#include "UIElement.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief	ビジュアルツリーの要素にアクセスするためのユーティリティです。
*/
class VisualTreeHelper
{
public:
	static UIElement* FindChildByName(UIElement* parent, const TCHAR* name);
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
