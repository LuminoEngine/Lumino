#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UIListBoxItem
    : public UICollectionItem
{
public:
    virtual const String& elementName() const  override { static String name = u"UIListBoxItem"; return name; }
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
	: public UIItemsControl
{
public:
    static Ref<UIListBox> create();

protected:
    virtual void onAddChild(UIElement* child) override;

LN_CONSTRUCT_ACCESS:
	UIListBox();
	void init();

private:
};

} // namespace ln

