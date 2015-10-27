
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN
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
LN_NAMESPACE_END
