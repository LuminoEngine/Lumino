#pragma once
#include "UIScrollView.hpp"

namespace ln {

class UIItemElement
	: public UIElement
{
LN_CONSTRUCT_ACCESS:
	UIItemElement();
	void init();

private:
};

// Item を ItemElement でラップして扱う。
// ただ単に子要素を並べるだけであれば、ScrollView に addChild するだけでよいが、
// こちらは ItemElement を介して選択中であるとか、枠線を付けたりとかする。
class UIItemContainerElement
	: public UIScrollViewer
{
public:
	UIItemContainerElement();
	void init();

private:
};

// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
class UIListBox
	: public UIItemContainerElement
{
LN_CONSTRUCT_ACCESS:
	UIListBox();
	void init();

private:
};

} // namespace ln

