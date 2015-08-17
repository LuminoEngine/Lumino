
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief	ビジュアルツリーの要素にアクセスするためのユーティリティです。
*/
class VisualTreeHelper
{
public:
	static UIElement* FindChildByName(UIElement* parent, const TCHAR* name);
};

} // namespace GUI
} // namespace Lumino
