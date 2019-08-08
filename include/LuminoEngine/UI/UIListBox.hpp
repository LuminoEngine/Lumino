#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UIListBoxItem
    : public UICollectionItem
{
public:
    // TODO: group

LN_CONSTRUCT_ACCESS:
    UIListBoxItem();
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

