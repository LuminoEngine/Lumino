#pragma once
#include "UIControl.hpp"

namespace ln {

// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
class UIListBox
	: public UIControl
{
LN_CONSTRUCT_ACCESS:
	UIListBox();
	void init();

private:
};

} // namespace ln

