#pragma once
#include "UIScrollView.hpp"
#include "UIItemsModel.hpp"

namespace ln {
class UIToggleButton;

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


class UITreeViewItem
	: public UIItemElement
{
protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UITreeViewItem();
	void init();

private:
	Ref<UIToggleButton> m_expanderButton;
};

class UITreeView
	: public UIItemContainerElement
{
public:
	void setModel(UIItemsViewModel* model);

LN_CONSTRUCT_ACCESS:
	UITreeView();
	void init();

private:
	Ref<UIItemsViewModel> m_model;
};

} // namespace ln

