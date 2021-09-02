
#include "Internal.hpp"
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/Controls/UITreeBox.hpp>
#include "../UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UITreeBoxItem
//
// LogicalParent は常に UITreeBox
// ----------
// 設計元である WPF とは異なる点となる。WPF の実装は "Expander".
// UITreeView 実装時も問題となったことだが、LogicalParent を Item にしてしまうと、
// 奇数番目にストライプな背景色を付けたり、カラムを付けて TreeListView のような実装を行うことが非常に難しくなってしまう。
// VisualStudio のソリューションエクスプローラーの選択アイテム背景色のように、横 Stretch に背景を塗りつぶすのもやりづらい。
// ※ソリューションエクスプローラーの実装は標準の TreeView ではない。
// 特に Item の左側への装飾の追加に大きな制約が伴う。
// また階層が非常に深くなりがちなので、イベントルーティングやレイアウトのオーバーヘッドも大きくなりやすい。

LN_OBJECT_IMPLEMENT(UITreeBoxItem, UIControl) {}

UITreeBoxItem::UITreeBoxItem()
	: m_ownerTreeBox(nullptr)
	, m_parentItem(nullptr)
	, m_layoutDepth(0)
{}

bool UITreeBoxItem::init()
{
	if (!UIControl::init()) return false;

	// TODO: Style
	setHorizontalContentAlignment(UIHAlignment::Left);

	return true;
}

UITreeBoxItem* UITreeBoxItem::addItem(const ln::String& text)
{
	if (LN_REQUIRE(m_ownerTreeBox)) return nullptr;

	auto item = makeObject<UITreeBoxItem>();
	item->addChild(text);
	addItemInternal(item);
	return item;
}

Ref<EventConnection> UITreeBoxItem::connectOnSubmit(Ref<UIGeneralEventHandler> handler)
{
	return m_onSubmit.connect(handler);
}

Size UITreeBoxItem::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	Size desiredSize = UIControl::measureOverride(layoutContext, constraint);

	// depth offset
	const float depthOffset = desiredSize.height * (m_layoutDepth);	// TODO: 仮幅。margin 使っていいかも？
	desiredSize.width += depthOffset;

	return desiredSize;
}

Size UITreeBoxItem::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	Rect contentRect = finalArea;

	const float depthOffset = contentRect.height * (m_layoutDepth);	// TODO: 仮幅。margin 使っていいかも？
	contentRect.x += depthOffset;
	contentRect.width -= depthOffset;

	return UIControl::arrangeOverride(layoutContext, contentRect);
}

void UITreeBoxItem::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		const auto* me = static_cast<const UIMouseEventArgs*>(e);
		m_ownerTreeBox->notifyItemClicked(this, me->getClickCount());
		e->handled = true;
		return;
	}
	else if (e->type() == UIEvents::MouseMoveEvent) {
		//if (m_ownerListControl->submitMode() == UIListSubmitMode::Single) {
		//	m_ownerListControl->selectItemExclusive(this);
		//	e->handled = true;
		//	return;
		//}
	}

	UIControl::onRoutedEvent(e);

}

void UITreeBoxItem::onSubmit()
{
	m_onSubmit.raise(UIEventArgs::create(this, UIEvents::Submitted));
}

void UITreeBoxItem::addItemInternal(UITreeBoxItem* item)
{
	if (LN_REQUIRE(!item->m_parentItem)) return;
	if (LN_REQUIRE(m_ownerTreeBox)) return;
	m_childItems.add(item);
	item->m_parentItem = this;

	// VisualChild としては直ちに追加しておく。
	// そうしないと、Style が適用されないため、measure でフォントサイズを元に要素サイズを計算できなくなる。
	m_ownerTreeBox->addVisualChild(item);

	m_ownerTreeBox->requireRefresh();
}

void UITreeBoxItem::traverse(int depth, const std::function<void(UITreeBoxItem*)> func)
{
	m_layoutDepth = depth;
	func(this);
	for (auto& item : m_childItems) {
		item->traverse(depth + 1, func);
	}
}

//==============================================================================
// UITreeBox

LN_OBJECT_IMPLEMENT(UITreeBox, UIControl) {}

UITreeBox::UITreeBox()
	: m_needRefresh(false)
{}

bool UITreeBox::init()
{
	if (!UIControl::init()) return false;
	return true;
}

UITreeBoxItem* UITreeBox::addItem(const ln::String& text, Ref<Variant> data)
{
	auto item = makeObject<UITreeBoxItem>();
	item->addChild(text);
	item->setData(data);
	addItemInternal(item);
	return item;
}

void UITreeBox::addItemInternal(UITreeBoxItem* item)
{
	if (LN_REQUIRE(item)) return;
	if (LN_REQUIRE(!item->logicalParent())) return;
	if (LN_REQUIRE(!item->m_ownerTreeBox)) return;

	m_rootItems.add(item);
	item->m_ownerTreeBox = this;

	addLogicalChild(item);
	addVisualChild(item);
}

Size UITreeBox::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	if (m_needRefresh) {
		refresh();
	}

	const Size contentSize = UIStackLayout::measureOverrideImpl(
		layoutContext,
		constraint,
		m_logicalChildren->size(),
		[this](int i) { return m_logicalChildren->at(i); },
		UILayoutOrientation::Vertical);
	return layoutContext->makeDesiredSize(this, contentSize);
}

Size UITreeBox::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const Rect contentRect = layoutContext->makeContentRect(this, finalArea.getSize());

	return UIStackLayout::arrangeOverrideImpl(
		layoutContext,
		contentRect,
		m_logicalChildren->size(),
		[this](int i) { return m_logicalChildren->at(i); },
		UILayoutOrientation::Vertical,
		finalStyle(),
		false,
		Vector2(0, 0));
}

void UITreeBox::requireRefresh()
{
	m_needRefresh = true;
}

void UITreeBox::refresh()
{
	removeAllLogicalChildren();
	const auto collect = [this](UITreeBoxItem* item) {
		addLogicalChild(item);
	};
	for (auto& item : m_rootItems) {
		item->traverse(0, collect);
	}

	// Layout の更新だけでよい。Style 更新は不要。
	invalidate(detail::UIElementDirtyFlags::Layout, true);

	m_needRefresh = false;
}

void UITreeBox::notifyItemClicked(UITreeBoxItem* item, int clickCount)
{
	item->onSubmit();
}

} // namespace ln

