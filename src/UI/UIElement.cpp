
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIStyle.h>
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIElement
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIElement, tr::ReflectionObject);

// Property definition
LN_TR_PROPERTY2_IMPLEMENT(UIElement, PointF, position, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, Size, size, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, AlignmentAnchor, anchor, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, HAlignment, hAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, VAlignment, vAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, BrushPtr, background, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, BrushPtr, foreground, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, BrushPtr, decoratorBackground, tr::PropertyMetadata());
LN_TR_PROPERTY2_IMPLEMENT(UIElement, float, decoratorOpacity, tr::PropertyMetadata());

// Event definition
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseEnterEvent, "MouseEnter", MouseEnter);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseLeaveEvent, "MouseLeave", MouseLeave);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyDownEvent, "KeyDown", KeyDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyUpEvent, "KeyUp", KeyUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, TextInputEvent, "TextInput", TextInput);

//------------------------------------------------------------------------------
UIElement::UIElement()
	: m_manager(nullptr)
	, m_parent(nullptr)
	, m_localStyle(nullptr)
	, m_currentVisualStateStyle(nullptr)
	, position(PointF(0, 0))
	, size(Size(NAN, NAN))
	, anchor(AlignmentAnchor::None)
	, hAlignment(HAlignment::Stretch)
	, vAlignment(VAlignment::Stretch)
	, m_opacity(1.0f)
	, decoratorBackground(nullptr)
	, decoratorOpacity(1.0f)
	, m_combinedOpacity(0.0f)
	, m_isEnabled(true)
	, m_isMouseOver(nullptr)
{
}

//------------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_localStyle);
}

//------------------------------------------------------------------------------
void UIElement::Initialize(detail::UIManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_invalidateFlags |= detail::InvalidateFlags::Initializing;

	// 要素名を覚えておく。末端のサブクラスの名前となる。
	m_elementName = tr::TypeInfo::GetTypeInfo(this)->GetName();

	m_localStyle = LN_NEW UIStylePropertyTable();

	GoToVisualState(String::GetEmpty());
}

//------------------------------------------------------------------------------
void UIElement::SetLayoutColumn(int index) { m_gridLayoutInfo.layoutColumn = index; }
int UIElement::GetLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
void UIElement::SetLayoutRow(int index) { m_gridLayoutInfo.layoutRow = index; }
int UIElement::GetLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
void UIElement::SetLayoutColumnSpan(int span) { m_gridLayoutInfo.layoutColumnSpan = span; }
int UIElement::GetLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
void UIElement::SetLayoutRowSpan(int span) { m_gridLayoutInfo.layoutRowSpan = span; }
int UIElement::GetLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }

//------------------------------------------------------------------------------
void UIElement::GoToVisualState(const StringRef& stateName)
{
	m_currentVisualStateName = stateName;
	m_invalidateFlags |= detail::InvalidateFlags::VisualState;
}

//------------------------------------------------------------------------------
void UIElement::Focus()
{
	if (IsFocusable())
	{
		if (m_parent != nullptr)
		{
			m_parent->ActivateInternal(this);
		}
	}
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void UIElement::CaptureMouse()
//{
//	m_ownerLayoutView->CaptureMouse(this);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void UIElement::ReleaseMouseCapture()
//{
//	m_ownerLayoutView->ReleaseMouseCapture(this);
//}
//
//------------------------------------------------------------------------------
int UIElement::GetVisualChildrenCount() const
{
	return 0;
}

//------------------------------------------------------------------------------
//UIElement* UIElement::GetVisualChild(int index) const
//{
//	LN_THROW(0, InvalidOperationException);
//	return nullptr;
//}

//------------------------------------------------------------------------------
UIElement* UIElement::GetVisualChildOrderd(int index) const
{
	LN_THROW(0, InvalidOperationException);
	return nullptr;
}

//------------------------------------------------------------------------------
void UIElement::RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e)
{
	e->sender = sender;
	RaiseEventInternal(ev, e);
}

//------------------------------------------------------------------------------
void UIElement::MeasureLayout(const Size& availableSize)
{
	ILayoutElement::MeasureLayout(availableSize);

	// フォントの無効フラグを落とす
	// TODO: UITextElement へ移動した方が良いかも？
	m_invalidateFlags &= ~detail::InvalidateFlags::Font;
}

//------------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalLocalRect)
{
	ILayoutElement::ArrangeLayout(finalLocalRect);

	OnLayoutUpdated();
}

//------------------------------------------------------------------------------
Size UIElement::MeasureOverride(const Size& constraint)
{
	return ILayoutElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UIElement::ArrangeOverride(const Size& finalSize)
{
	return ILayoutElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UIElement::OnLayoutUpdated()
{
}

//------------------------------------------------------------------------------
void UIElement::OnRender(DrawList* g)
{
	if (background.Get() != nullptr)
	{
		g->SetBrush(background.Get());
		//g->SetOpacity(m_combinedOpacity);
		g->DrawRectangle(RectF(0, 0, m_finalLocalRect.GetSize()));
	}
	if (decoratorBackground.Get() != nullptr)
	{
		g->SetBrush(decoratorBackground.Get());
		//g->SetOpacity(m_combinedOpacity * m_decoratorOpacity);
		g->DrawRectangle(RectF(0, 0, m_finalLocalRect.GetSize()));
	}
}

//------------------------------------------------------------------------------
void UIElement::OnMouseMove(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseMoveEvent, this, e); } }
void UIElement::OnMouseDown(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseDownEvent, this, e); } }
void UIElement::OnMouseUp(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseUpEvent, this, e); } }
void UIElement::OnKeyDown(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(KeyDownEvent, this, e); } }
void UIElement::OnKeyUp(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(KeyUpEvent, this, e); } }
void UIElement::OnTextInput(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(TextInputEvent, this, e); } }

//------------------------------------------------------------------------------
void UIElement::OnMouseEnter(UIMouseEventArgs* e)
{
	// 親にもマウスがはじめて乗ったのであれば親にも通知する
	if (m_parent != NULL && !m_parent->m_isMouseOver) {
		m_parent->OnMouseEnter(e);
	}

	m_isMouseOver = true;

	if (!e->handled) { RaiseEvent(MouseEnterEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::OnMouseLeave(UIMouseEventArgs* e)
{
	// 親にもマウスが乗ったことになっていれば、ヒットテストをした上で通知する
	if (m_parent != NULL && m_parent->m_isMouseOver)
	{
		if (!m_parent->m_finalGlobalRect.Contains(PointF(e->x, e->y))) {
			m_parent->OnMouseLeave(e);
		}
	}

	m_isMouseOver = false;

	if (!e->handled) { RaiseEvent(MouseLeaveEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::SetParent(UIElement* parent)
{
	if (parent != nullptr)
	{
		// 既に親があるとき、新しい親をつけることはできない
		LN_THROW(GetParent() == nullptr, InvalidOperationException, "the child elements of already other elements.");
	}

	m_parent = parent;
}

//------------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	int count = GetVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		UIElement* e = GetVisualChildOrderd(i)->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}

	if (m_finalGlobalRect.Contains(globalPt)) {
		return this;
	}
	return NULL;
}

//------------------------------------------------------------------------------
// child : Activate の発生元となった UIElement
void UIElement::ActivateInternal(UIElement* child)
{
	if (m_parent != nullptr)
	{
		m_parent->ActivateInternal(child);
	}
}

//------------------------------------------------------------------------------
bool UIElement::OnEvent(detail::UIInternalEventType type, UIEventArgs* args)
{
	/* 今のところ、イベントを再帰で通知していく必要はない。
	マウスイベントは Hover しているものへ Manager が直接送り込む。
	キーイベントはフォーカスを持っているものへ Manager が直接送り込む。

	…というか、再帰で通知してはならない。マウスイベントとかは再帰してしまうと、
	マウスカーソルが乗っていない要素がイベントを受け取ってしまうことになる。
	*/
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	//int count = GetVisualChildrenCount();
	//for (int i = count - 1; i >= 0; i--)
	//{
	//	if (GetVisualChild(i)->OnEvent(type, args)) { return true; }
	//}


	switch (type)
	{
	case detail::UIInternalEventType::Unknown:
		break;
	case detail::UIInternalEventType::MouseMove:
		if (m_isEnabled) { OnMouseMove(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseButtonDown:
		if (m_isEnabled) { OnMouseDown(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseButtonUp:
		if (m_isEnabled) { OnMouseUp(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseWheel:
		break;
	case detail::UIInternalEventType::KeyDown:
		if (m_isEnabled) { OnKeyDown(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::KeyUp:
		if (m_isEnabled) { OnKeyUp(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::TextInput:
		if (m_isEnabled) { OnTextInput(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::ElapsedTime:
		break;
	case detail::UIInternalEventType::MouseEnter:	// TODO: 親領域
		if (m_isEnabled) { OnMouseEnter(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseLeave:	// TODO: 親領域
		if (m_isEnabled) { OnMouseLeave(static_cast<UIMouseEventArgs*>(args)); }
		break;
	default:
		break;
	}

	//if (type == EventType_MouseMove) {
	//	//RaiseEvent<MouseEventArgs>(MouseMoveEvent, this, (MouseEventArgs*)args);
	//}
	return args->handled;
}

//------------------------------------------------------------------------------
// this でハンドリングしたいルーティングイベントが発生したとき、イベント経由ではなく
// 通常の仮想関数で通知することで、パフォーマンスの向上を図る。
// (UI要素作成時のイベントハンドラの new や AddHandler をする必要がなくなる)
void UIElement::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
}

//------------------------------------------------------------------------------
//void UIElement::ApplyTemplate()
//{
//	/// 現在のテンプレートからビジュアルツリーを再構築します。
//	/// この関数は必要なタイミングでレイアウトシステムから呼び出されます。通常、明示的に呼び出す必要はありません。
//	///		というか、呼び出しちゃダメ。必ずルートから再帰的に更新しないと、もし親がまだ ApplyTemplate() してない状態でこれを呼ぶと
//	///		ローカルリソースが正しく更新されない。
//	///		TODO: もしかしたら、SetParent した瞬間にローカルリソースを更新したほうが良いかも？
//	///		そうすればいつ ApplyTemplate() を呼び出しても良いが… 需要は無いか。
//
//
//	// VisualState の変更
//	if (m_invalidateFlags.TestFlag(detail::InvalidateFlags::VisualState))
//	{
//
//
//		m_invalidateFlags &= ~detail::InvalidateFlags::VisualState;
//	}
//
//	ApplyTemplateHierarchy();
//}

//------------------------------------------------------------------------------
void UIElement::ApplyTemplateHierarchy(UIStyleTable* styleTable, UIStylePropertyTable* parentStyle)
{
	// TODO: styleTable は多分 Context のルート固定でよい。

	// VisualState の変更がある場合
	if (m_invalidateFlags.TestFlag(detail::InvalidateFlags::VisualState))
	{
		UIStyle* style = styleTable->FindStyle(tr::TypeInfo::GetTypeInfo(this));
		if (style != nullptr)
		{
			m_currentVisualStateStyle = style->FindStylePropertyTable(m_currentVisualStateName);
		}
		else
		{
			m_currentVisualStateStyle = nullptr;
		}
		m_invalidateFlags &= ~detail::InvalidateFlags::VisualState;
	}

	// Style 更新
	UpdateLocalStyleAndApplyProperties(parentStyle, m_currentVisualStateStyle);

	// 子要素
	UIStylePropertyTable* localStyle = m_localStyle;
	UIHelper::ForEachVisualChildren(this, [styleTable, localStyle](UIElement* child) { child->ApplyTemplateHierarchy(styleTable, localStyle); });
}

//------------------------------------------------------------------------------
void UIElement::UpdateLocalStyleAndApplyProperties(UIStylePropertyTable* parentStyle, UIStylePropertyTable* currentStateStyle)
{
	LN_CHECK_STATE(m_localStyle != nullptr);

	// parent → state の順で local へマージする
	// TODO: このへんのコピーが時間かかりそうならリビジョンカウント使うとか対策する。毎フレームやってるから多分重い。
	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;
	if (parentStyle != nullptr)       invalidate |= m_localStyle->UpdateInherit(parentStyle);
	if (currentStateStyle != nullptr) invalidate |= m_localStyle->UpdateInherit(currentStateStyle);

	if (invalidate != detail::InvalidateFlags::None)
	{
		OnUpdateStyle(m_localStyle, invalidate);
	}
}

//------------------------------------------------------------------------------
void UIElement::OnUpdateStyle(UIStylePropertyTable* localStyle, detail::InvalidateFlags invalidateFlags)
{
	localStyle->Apply(this, !m_invalidateFlags.TestFlag(detail::InvalidateFlags::Initializing));
	// TODO: UITextElement::OnUpdateStyle 参照
	// TODO: アニメーション
	//if (tr::Property::GetBaseValueSource(this, BackgroundProperty) <= tr::PropertySetSource::ByStyle)
	//	tr::Property::SetPropertyValueDirect<BrushPtr>(this, BackgroundProperty, localStyle->m_background.value, tr::PropertySetSource::ByStyle);
	//if (tr::Property::GetBaseValueSource(this, ForegroundProperty) <= tr::PropertySetSource::ByStyle)
	//	tr::Property::SetPropertyValueDirect<BrushPtr>(this, ForegroundProperty, localStyle->m_foreground.value, tr::PropertySetSource::ByStyle);
}

//------------------------------------------------------------------------------
void UIElement::OnUpdatingLayout()
{
	// 子要素
	UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->OnUpdatingLayout(); });
}

//------------------------------------------------------------------------------
void UIElement::UpdateLayout(const Size& viewSize)
{
	Size size(
		Math::IsNaNOrInf(size.Get().width) ? viewSize.width : size.Get().width,
		Math::IsNaNOrInf(size.Get().height) ? viewSize.height : size.Get().height);

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	OnUpdatingLayout();

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.width, size.height));
}

//------------------------------------------------------------------------------
void UIElement::UpdateTransformHierarchy()
{
	if (m_parent != nullptr)
	{
		m_finalGlobalRect.x = m_parent->m_finalGlobalRect.x + m_finalLocalRect.x;
		m_finalGlobalRect.y = m_parent->m_finalGlobalRect.y + m_finalLocalRect.y;
		m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
	}
	else
	{
		m_finalGlobalRect.x = m_finalLocalRect.x;
		m_finalGlobalRect.y = m_finalLocalRect.y;
		m_combinedOpacity = m_opacity;
	}
	m_finalGlobalRect.width = m_finalLocalRect.width;
	m_finalGlobalRect.height = m_finalLocalRect.height;

	// 子要素
	UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });

	// 最初の更新おわり
	m_invalidateFlags &= ~detail::InvalidateFlags::Initializing;
	m_invalidateFlags &= ~detail::InvalidateFlags::ParentChangedUpdating;
}

//------------------------------------------------------------------------------
void UIElement::Render(DrawList* g)
{
	Matrix mat;
	mat.Translate(m_finalGlobalRect.x, m_finalGlobalRect.y, 0);
	g->SetTransform(mat);

	OnRender(g);

	// 子要素
	UIHelper::ForEachVisualChildren(this, [g](UIElement* child) { child->Render(g); });
}

//------------------------------------------------------------------------------
const HAlignment* UIElement::GetPriorityContentHAlignment()
{
	return nullptr;
}

//------------------------------------------------------------------------------
const VAlignment* UIElement::GetPriorityContentVAlignment()
{
	return nullptr;
}

//------------------------------------------------------------------------------
void UIElement::RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e)
{
	LN_CHECK_ARG(ev != nullptr);
	LN_CHECK_ARG(e != nullptr);

	// まずは this に通知
	OnRoutedEvent(ev, e);
	if (e->handled) return;

	// this に AddHandler されているイベントハンドラを呼び出す
	tr::TypeInfo* thisType = tr::TypeInfo::GetTypeInfo(this);
	thisType->InvokeReflectionEvent(this, ev, e);
	if (e->handled) return;

	// bubble
	if (m_parent != nullptr)
	{
		m_parent->RaiseEventInternal(ev, e);
	}
}

//------------------------------------------------------------------------------
const PointF& UIElement::GetLayoutPosition() const { return position; }
const Size& UIElement::GetLayoutSize() const { return size; }
const ThicknessF& UIElement::GetLayoutMargin() const { return m_margin; }
const ThicknessF& UIElement::GetLayoutPadding() const { return m_padding; }
AlignmentAnchor UIElement::GetLayoutAnchor() const { return anchor; }
HAlignment UIElement::GetLayoutHAlignment() const { return hAlignment; }
VAlignment UIElement::GetLayoutVAlignment() const { return vAlignment; }
ILayoutElement* UIElement::GetLayoutParent() const { return m_parent; }
const HAlignment* UIElement::GetLayoutContentHAlignment() { return GetPriorityContentHAlignment(); }
const VAlignment* UIElement::GetLayoutContentVAlignment() { return GetPriorityContentVAlignment(); }
const Size& UIElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void UIElement::SetLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void UIElement::SetLayoutFinalLocalRect(const RectF& rect) { m_finalLocalRect = rect; }
//int UIElement::GetLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
//int UIElement::GetLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
//int UIElement::GetLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
//int UIElement::GetLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }

LN_NAMESPACE_END
