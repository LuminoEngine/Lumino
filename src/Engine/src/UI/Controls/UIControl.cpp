
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/Controls/UIControl.hpp>
#include <LuminoEngine/UI/UIActiveTimer.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include "../UIStyleInstance.hpp"
#include "../UIManager.hpp"

namespace ln {

//==============================================================================
// UIControl

LN_OBJECT_IMPLEMENT(UIControl, UIElement) {}

UIControl::UIControl()
    : m_enabledDirectChildrenContentAlignment(true)
	, m_logicalChildren(makeCollection<Ref<UIElement>>())
{
	m_objectManagementFlags.set(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
    m_specialElementFlags.set(detail::UISpecialElementFlags::Control);
    setFocusable(true);
}

bool UIControl::init()
{
    if (!UIElement::init()) return false;
    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Normal);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::MouseOver);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Disabled);
    vsm->registerState(UIVisualStates::FocusStates, UIVisualStates::Focused);
    vsm->registerState(UIVisualStates::FocusStates, UIVisualStates::Unfocused);
    vsm->registerState(UIVisualStates::Visibility, UIVisualStates::Visible);
    vsm->registerState(UIVisualStates::Visibility, UIVisualStates::Hidden);
    vsm->registerState(UIVisualStates::Visibility, UIVisualStates::Collapsed);
    vsm->gotoState(UIVisualStates::Normal);
    vsm->gotoState(UIVisualStates::Unfocused);
    vsm->gotoState(UIVisualStates::Visible);

    return true;
}

void UIControl::onDispose(bool explicitDisposing)
{
    removeAllChildren();
	UIElement::onDispose(explicitDisposing);
}

void UIControl::setHorizontalContentAlignment(UIHAlignment value)
{
    m_localStyle->mainStyle()->horizontalContentAlignment = value;
}

UIHAlignment UIControl::horizontalContentAlignment() const
{
    return m_localStyle->mainStyle()->horizontalContentAlignment;
}

void UIControl::setVerticalContentAlignment(UIVAlignment value)
{
    m_localStyle->mainStyle()->verticalContentAlignment = value;
}

UIVAlignment UIControl::verticalContentAlignment() const
{
    return m_localStyle->mainStyle()->verticalContentAlignment;
}

void UIControl::addElement(UIElement* element)
{
	if (LN_REQUIRE(element)) return;

    // 通常は作成直後、デフォルトの Container に追加されている。
    // WPF のように別の親に追加済みであれば例外するのもありだけど、いちいち removeFromParent するのは面倒。
    if (element->logicalParent()) {
        element->removeFromLogicalParent();
    }
	
    m_logicalChildren->add(element);
    element->setLogicalParent(this);
	addVisualChild(element);

	//if (m_logicalChildrenHost) {
	//	m_logicalChildrenHost->addLayoutOwnerLogicalChild(element);
	//}
}

void UIControl::removeElement(UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    if (LN_REQUIRE(element->logicalParent() == this)) return;

    //if (m_logicalChildrenHost) {
    //    m_logicalChildrenHost->removeLayoutOwnerLogicalChild(element);
    //}

    m_logicalChildren->remove(element);
    element->setLogicalParent(nullptr);
	removeVisualChild(element);
    onLogicalChildRemoved(element);
}

void UIControl::removeAllChildren()
{
	for (auto& e : *m_logicalChildren) {
		removeVisualChild(e);
	}

	m_logicalChildren->clear();
	//if (m_logicalChildrenHost) {
	//	m_logicalChildrenHost->clearLayoutOwnerLogicalChildren();
	//}
}

void UIControl::addInlineVisual(UIElement* element, UIInlinePlacement layout)
{
    PointI pts[] = {
        { 0, 0 }, { 1, 0 }, { 2, 0 },
        { 0, 1 }, { 1, 1 }, { 2, 1 },
        { 0, 2 }, { 1, 2 }, { 2, 2 },
    };
    element->getGridLayoutInfo()->layoutRow = pts[(int)layout].y;
    element->getGridLayoutInfo()->layoutColumn = pts[(int)layout].x;
    m_inlineElements.add(element);
    addVisualChild(element);

    if (!m_aligned3x3GridLayoutArea) {
        m_aligned3x3GridLayoutArea = makeObject<detail::UIAligned3x3GridLayoutArea>();
    }
}

void UIControl::registerActiveTimer(UIActiveTimer* timer)
{
	if (LN_REQUIRE(timer)) return;
	if (LN_REQUIRE(!timer->m_owner)) return;
	m_activeTimers.add(timer);
	timer->m_owner = this;
	detail::EngineDomain::uiManager()->registerActiveTimer(timer);
}

void UIControl::unregisterActiveTimer(UIActiveTimer* timer)
{
	if (LN_REQUIRE(timer)) return;
	if (LN_REQUIRE(timer->m_owner == this)) return;
	m_activeTimers.remove(timer);
	timer->m_owner = nullptr;
	detail::EngineDomain::uiManager()->unregisterActiveTimer(timer);
}

void UIControl::addAction(UIAction* action)
{
    if (!m_actions) {
        m_actions = ln::makeList<Ref<UIAction>>();
    }
    m_actions->add(action);
}

Ref<EventConnection> UIControl::connectOnActivated(Ref<UIEventHandler> handler)
{
    return m_onActivated.connect(handler);
}

Ref<EventConnection> UIControl::connectOnDeactivated(Ref<UIEventHandler> handler)
{
    return m_onDeactivated.connect(handler);
}

void UIControl::onActivated()
{
    m_onActivated.raise(/*UIEventArgs::create(this, UIEvents::Activated)*/);
}

void UIControl::onDeactivated()
{
    m_onDeactivated.raise(/*UIEventArgs::create(this, UIEvents::Deactivated)*/);
}

void UIControl::onLogicalChildRemoved(UIElement* removedElement)
{
}

void UIControl::onAddChild(UIElement* child)
{
    addElement(child);
}

void UIControl::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseEnterEvent) {
        getVisualStateManager()->gotoState(UIVisualStates::MouseOver);
        e->handled = true;
        return;
    }
    else if (e->type() == UIEvents::MouseLeaveEvent) {
        getVisualStateManager()->gotoState(UIVisualStates::Normal);
    }
    else if (e->type() == UIEvents::LostFocusEvent) {
        getVisualStateManager()->gotoState(UIVisualStates::Unfocused);
    }
    else if (e->type() == UIEvents::GotFocusEvent) {
        getVisualStateManager()->gotoState(UIVisualStates::Focused);
    }

    if (m_actions) {
        if (detail::UICommandInternal::handleCommandRoutedEvent(e, *m_actions)) {
            return;
        }
    }

    UIElement::onRoutedEvent(e);
}

Size UIControl::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    Size size;

    if (m_aligned3x3GridLayoutArea) {
        // 論理子要素の領域 (content area)
		Size childrenAreaSize = detail::LayoutHelper::UIFrameLayout_staticMeasureChildrenAreaSize(layoutContext, m_logicalChildren, constraint);//UIFrameLayout2::staticMeasureChildrenAreaSize(m_logicalChildren, constraint);
        // Inline 要素も含めた領域 (client area)
        Size clientAreaSize = m_aligned3x3GridLayoutArea->measure(layoutContext, m_inlineElements, constraint, childrenAreaSize);
        // padding, border も含めたサイズ (client は、this と clientAreaSize のうち大きい方を採用)
        size = layoutContext->makeDesiredSize(this, clientAreaSize);
    }
    else if (m_autoLayoutLogicalChildren) {
        //   struct ElementList : public IUIElementList {
        //       List<Ref<UIElement>>* list;
        //       virtual int getElementCount() const { return list->size(); }
        //       virtual UIElement* getElement(int i) const { return list->at(i); }
        //   } list;
        //   list.list = &m_logicalChildren;

           //UILayoutPanel2_Deprecated* layout = layoutPanel();
           //layout->measureLayout(&list, constraint);
        //   Size desiredSize = layout->desiredSize();
        //   Size localSize = UIElement::measureOverride(constraint);
        //   return Size::max(desiredSize, localSize);

           //if (m_logicalChildrenHost) {
        //       m_logicalChildrenHost->measureLayout(constraint);
        //       Size layoutSize = m_logicalChildrenHost->desiredSize();
        //       Size localSize = UIElement::measureOverride(constraint);
        //       return Size::max(layoutSize, localSize);
           //}
           //else {
        size = UIFrameLayout2::staticMeasureLogicalChildren(layoutContext, this, constraint);

        //}
    }
    else {
        size = UIElement::measureOverride(layoutContext, constraint);
    }

    // デフォルトの最小サイズ
    if (const UITheme* theme = layoutContext->styleContext()->mainTheme) {
        size.height = std::max(size.height, theme->lineContentHeight());
    }
    return size;
}

Size UIControl::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    //Rect finalArea(0, 0, finalSize);

    if (m_aligned3x3GridLayoutArea) {
        // padding, border を考慮した領域を計算
        Rect clientArea = detail::LayoutHelper::arrangeClientArea(this, finalArea);
        // Inline 要素を arrange & 論理子要素の領域 (content area) を計算
        Rect contentArea;
        m_aligned3x3GridLayoutArea->arrange(layoutContext, m_inlineElements, clientArea, &contentArea);
        // 論理子要素を arrange
		detail::LayoutHelper::UIFrameLayout_staticArrangeChildrenArea(
            layoutContext, this,
            m_finalStyle->horizontalContentAlignment,
            m_finalStyle->verticalContentAlignment,
            m_logicalChildren, contentArea);
        //UIFrameLayout2::staticArrangeChildrenArea(this, m_logicalChildren, contentArea);

        return finalArea.getSize();
    }
    else if (m_autoLayoutLogicalChildren) {
        return UIFrameLayout2::staticArrangeLogicalChildren(layoutContext, this, finalArea);
    }
    else {
        return UIElement::arrangeOverride(layoutContext, finalArea);
    }
}

// activateInternal と deactivateInternal は、以前は UIElement が持っていた。しかし、
// - 見た目だけを表現したい Shape をクリックしたときに前後関係が変わってほしくない。
// - onActivate/onDeactivate を Event にしたいが、UIElement に Event を持たせるとまたメモリサイズが増える
// 実際のところ onActivate/onDeactivate は Control の機能で十分と判断したのでこっちに持ってきている。
void UIControl::activateInternal()
{
    if (m_visualParent && specialElementFlags().hasFlag(detail::UISpecialElementFlags::FloatingSiblingOrder)) {
        m_visualParent->moveVisualChildToForeground(this);
    }

    onActivated();
}

void UIControl::deactivateInternal()
{
	//if (m_visualParent) {
	//	m_visualParent->deactivateInternal();
	//}

    onDeactivated();
}

//==============================================================================
// UIAligned3x3GridLayout

namespace detail {

UIAligned3x3GridLayoutArea::UIAligned3x3GridLayoutArea()
{
}

void  UIAligned3x3GridLayoutArea::init()
{
    Object::init();
}

Size UIAligned3x3GridLayoutArea::measure(UILayoutContext* layoutContext, const List<Ref<UIElement>>& inlineElements, const Size& constraint, const Size& contentDesiredSize)
{
#if 1
    // 各セルの desiredSize を測定
    for (int i = 0; i < inlineElements.size(); i++) {
        UIElement* child = inlineElements[i];
        child->measureLayout(layoutContext, constraint);
        const Size& childDesiredSize = child->getLayoutDesiredSize();

        int row, column, rowSpan, columnSpan;
        getGridInfoHelper(child, &row, &column, &rowSpan, &columnSpan);

        m_rows[row].desiredSize = std::max(m_rows[row].desiredSize, childDesiredSize.height);
        m_columns[column].desiredSize = std::max(m_columns[column].desiredSize, childDesiredSize.width);
    }

    // contentSize を中央のセルとして計算する
    m_rows[1].desiredSize = std::max(m_rows[1].desiredSize, contentDesiredSize.height);
    m_columns[1].desiredSize = std::max(m_columns[1].desiredSize, contentDesiredSize.width);

    // 全体の desiredSize を測定
    Size desiredSize;
    for (int i = 0; i < 3; i++) {
        desiredSize.height += m_rows[i].desiredSize;
        desiredSize.width += m_columns[i].desiredSize;
    }

    return desiredSize;
#else
    for (int i = 0; i < inlineElements.size(); i++)
    {
        UIElement* child = inlineElements[i];
        child->measureLayout(layoutContext, constraint);
        const Size& childDesiredSize = child->getLayoutDesiredSize();

        int row, column, rowSpan, columnSpan;
        getGridInfoHelper(child, &row, &column, &rowSpan, &columnSpan);

        // span を考慮し、この child が影響するセル範囲の 右と下 の分割線の位置を押し上げるように領域の調整を行う
        int br = std::min(std::max(row, 0), 3);
        int bc = std::min(std::max(column, 0), 3);
        int tr = std::min(std::max(row + rowSpan - 1, 0), 3);
        int tc = std::min(std::max(column + columnSpan - 1, 0), 3);
        float leftLineOffset = (br == 0) ? 0 : m_rows[br - 1].desiredLastOffset;
        float topLineOffset = (bc == 0) ? 0 : m_columns[bc - 1].desiredLastOffset;
        m_rows[tr].desiredLastOffset = std::max(m_rows[tr].desiredLastOffset, leftLineOffset + childDesiredSize.height);
        m_columns[tc].desiredLastOffset = std::max(m_columns[tr].desiredLastOffset, topLineOffset + childDesiredSize.width);
    }

    // contentSize を中央のセルとして計算する
    m_rows[1].desiredLastOffset = std::max(m_rows[1].desiredLastOffset, m_rows[0].desiredLastOffset + contentDesiredSize.height);
    m_columns[1].desiredLastOffset = std::max(m_columns[1].desiredLastOffset, m_columns[0].desiredLastOffset + contentDesiredSize.width);

    // 各セルの desiredSize を確定させる
    for (int i = 0; i < 3; i++)
    {
        float pr = (i == 0) ? 0.0f : m_rows[i - 1].desiredLastOffset;
        float pc = (i == 0) ? 0.0f : m_columns[i - 1].desiredLastOffset;

        if (m_rows[i].desiredLastOffset < pr) m_rows[i].desiredLastOffset = pr;
        if (m_columns[i].desiredLastOffset < pc) m_columns[i].desiredLastOffset = pc;

        m_rows[i].desiredSize = m_rows[i].desiredLastOffset - pr;
        m_columns[i].desiredSize = m_columns[i].desiredLastOffset - pc;
    }

    // 計算が終わると、右端と下端の次の分割線の位置がサイズとみなせる
    return Size(m_columns[2].desiredLastOffset, m_rows[2].desiredLastOffset);
#endif
}

void UIAligned3x3GridLayoutArea::arrange(UILayoutContext* layoutContext, const List<Ref<UIElement>>& inlineElements, const Rect& finalArea, Rect* outActualContentRect)
{
#if 0
    // 両端は Auto 扱い。desiredSize を使ってそのまま確定。
    m_rows[0].actualSize = m_rows[0].desiredSize;
    m_rows[2].actualSize = m_rows[2].desiredSize;
    m_columns[0].actualSize = m_columns[0].desiredSize;
    m_columns[2].actualSize = m_columns[2].desiredSize;

#else
    LN_CHECK(outActualContentRect);

    // 両端は Auto 扱い。desiredSize を使ってそのまま確定。
    m_rows[0].actualSize = m_rows[0].desiredSize;
    m_rows[2].actualSize = m_rows[2].desiredSize;
    m_columns[0].actualSize = m_columns[0].desiredSize;
    m_columns[2].actualSize = m_columns[2].desiredSize;

    // 中心は Ratio. ただ1つしかないので残った領域をそのまま使う
    m_rows[1].actualSize = finalArea.height - m_rows[0].actualSize - m_rows[2].actualSize;
    m_columns[1].actualSize = finalArea.width - m_columns[0].actualSize - m_columns[2].actualSize;

    // actualOffset を確定させる
    Vector2 offset(0.0f, 0.0f);
    for (int i = 0; i < 3; i++)
    {
        m_rows[i].actualOffset = offset.y;
        m_columns[i].actualOffset = offset.x;
        offset.y += m_rows[i].actualSize;
        offset.x += m_columns[i].actualSize;
    }

    // arrange
    for (int i = 0; i < inlineElements.size(); i++)
    {
        UIElement* child = inlineElements[i];
        int row, column, rowSpan, columnSpan;
        getGridInfoHelper(child, &row, &column, &rowSpan, &columnSpan);

        Rect rect(finalArea.x + m_columns[column].actualOffset, finalArea.y + m_rows[row].actualOffset, 0, 0);
        for (int i = 0; i < rowSpan; i++) {
            rect.height += m_rows[row + i].actualSize;
        }
        for (int i = 0; i < columnSpan; i++) {
            rect.width += m_columns[column + i].actualSize;
        }

        child->arrangeLayout(layoutContext, rect);
    }

    // content area (呼び出し側でレイアウトするのに使う)
    outActualContentRect->x = finalArea.x + m_columns[1].actualOffset;
    outActualContentRect->y = finalArea.y + m_rows[1].actualOffset;
    outActualContentRect->width = m_columns[1].actualSize;
    outActualContentRect->height = m_rows[1].actualSize;
#endif
}

void UIAligned3x3GridLayoutArea::getGridInfoHelper(UIElement* element, int* row, int* column, int* rowSpan, int* columnSpan) const
{
    auto* info = element->m_gridLayoutInfo.get();
    if (info) {
        *row = std::min(std::max(info->layoutRow, 0), (int)m_rows.size());
        *column = std::min(std::max(info->layoutColumn, 0), (int)m_columns.size());
        *rowSpan = ((*row) + info->layoutRowSpan > (m_rows.size() + 1)) ? (m_rows.size() + 1) - (*row) : info->layoutRowSpan;
        *columnSpan = ((*column) + info->layoutColumnSpan > (m_columns.size() + 1)) ? (m_columns.size() + 1) - (*column) : info->layoutColumnSpan;
    }
    else {
        *row = 0;
        *column = 0;
        *rowSpan = 1;
        *columnSpan = 1;
    }
}

} // namespace detail


} // namespace ln

