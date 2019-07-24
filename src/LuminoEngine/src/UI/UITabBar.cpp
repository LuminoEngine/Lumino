
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UITabBar.hpp>

namespace ln {

//==============================================================================
// UITabItem

UITabItem::UITabItem()
{
}

void UITabItem::init()
{
	UICollectionItem::init();
}

//==============================================================================
// UITabBar
//	WPF の TabItem は HeaderedContentControl の派生であり、ラベル部分とコンテンツ部分がひとつの Control に統合されている。
//	UE4 は SDockTab。WPF と同じく、Header と Content が統合されているようだ。
//	一方 Qt や主要な CSS フレームワークでは、Tab と Content は独立している。
//  Lumino としては分ける方向で行ってみるが、QTabWidget のように統合されたユーティリティを用意してもいいだろう。
//	分ける用途としてはカスタマイズ性だが、例えば昨今のブラウザの TabBar と Content の間に AddressBar を入れたいようなときに使う。
//
//	クローズボタンは、CloseEvent を RoutedEvent で上に投げる。

UITabBar::UITabBar()
{
}

void UITabBar::init()
{
	UIItemsControl::init();

	auto layout = makeObject<UIStackLayout2>();
	layout->setOrientation(Orientation::Horizontal);
	setLayoutPanel(layout);
}

void UITabBar::addTab(UITabItem* item)
{
	addItem(item);
}

} // namespace ln

