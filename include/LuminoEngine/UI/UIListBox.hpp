#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UIListBoxItem
    : public UICollectionItem
{
public:
    virtual const String& elementName() const  override { static String name = u"UIListBoxItem"; return name; }
    // TODO: group


	void bind(ObservablePropertyBase* prop);

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

	// この UIElement のメイン要素と bind する Property を設定する。
	// 具体的にどのような表示要素と bind するのかは実装により異なる。
	// 例えば ListBox なら、Collection を bind することで ListBoxItem を生成する。
	// LsitBoxItem なら、デフォルトで作成される TextBlock に bind する。
	// ListBoxItem をカスタマイズしたいときは、今は未実装だが、generate() みたいなのをユーザープログラムで実装し、そのなかで main の prop を参照することになりそう。
	// WPF でいうところの DataContext = prop.
	void bind(ObservablePropertyBase* prop);

protected:
    virtual void onAddChild(UIElement* child) override;



LN_CONSTRUCT_ACCESS:
	UIListBox();
	void init();

private:
};

} // namespace ln

