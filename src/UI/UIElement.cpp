
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/DrawingContext.h>
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
LN_TR_PROPERTY_IMPLEMENT(UIElement, VerticalAlignment, VerticalAlignmentProperty, "VerticalAlignment", m_verticalAlignment, tr::PropertyMetadata(VerticalAlignment::Center));
LN_TR_PROPERTY_IMPLEMENT(UIElement, HorizontalAlignment, HorizontalAlignmentProperty, "HorizontalAlignment", m_horizontalAlignment, tr::PropertyMetadata(HorizontalAlignment::Center));
LN_TR_PROPERTY_IMPLEMENT(UIElement, BrushPtr, BackgroundProperty, "Background", m_background, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, BrushPtr, ForegroundProperty, "Foreground", m_foreground, tr::PropertyMetadata());

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
	, m_ownerLayoutView(nullptr)
	, m_parent(nullptr)
	, m_localStyle(nullptr)
	, m_currentVisualStateStyle(nullptr)
	, m_size(NAN, NAN)
	, m_horizontalAlignment(HorizontalAlignment::Center)
	, m_verticalAlignment(VerticalAlignment::Center)
	, m_opacity(1.0f)
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

	// 要素名を覚えておく。末端のサブクラスの名前となる。
	m_elementName = tr::TypeInfo::GetTypeInfo(this)->GetName();

	m_localStyle = LN_NEW UIStylePropertyTable();

	GoToVisualState(String::GetEmpty());
}

//------------------------------------------------------------------------------
void UIElement::GoToVisualState(const StringRef& stateName)
{
	m_currentVisualStateName = stateName;
	m_invalidateFlags = detail::InvalidateFlags::VisualState;
}

//------------------------------------------------------------------------------
void UIElement::Focus()
{
	if (IsFocusable())
	{
		if (m_parent != nullptr) { m_parent->ActivateInternal(this); }
		m_ownerLayoutView->GetOwnerContext()->SetFocusElement(this);
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
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	//// 無効情報フラグをこの要素に伝播させる
	//if (m_parent != nullptr)
	//{
	//	// フォントは MeasureOverride() の中で更新する
	//	m_invalidateFlags |= (m_parent->m_invalidateFlags & detail::InvalidateFlags::Font);
	//}

	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// ① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin を考慮する
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	SizeF localAvailableSize(
		std::max(availableSize.width - marginWidth, 0.0f),
		std::max(availableSize.height - marginHeight, 0.0f));

	m_desiredSize = MeasureOverride(localAvailableSize);

	// Margin を考慮する
	m_desiredSize.width += marginWidth;
	m_desiredSize.height += marginHeight;

	// フォントの無効フラグを落とす
	// TODO: UITextElement へ移動した方が良いかも？
	m_invalidateFlags &= ~detail::InvalidateFlags::Font;
}

//------------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	SizeF arrangeSize;

	// この要素のサイズが明示的に指定されている場合はそちらを優先する
	arrangeSize.width = Math::IsNaNOrInf(m_size.width) ? finalLocalRect.width : m_size.width;
	arrangeSize.height = Math::IsNaNOrInf(m_size.height) ? finalLocalRect.height : m_size.height;

	const SizeF& ds = GetDesiredSize();
	RectF arrangeRect;
	UIHelper::AdjustHorizontalAlignment(arrangeSize, ds, m_horizontalAlignment, &arrangeRect);
	UIHelper::AdjustVerticalAlignment(arrangeSize, ds, m_verticalAlignment, &arrangeRect);

	// Margin を考慮する (0 以下には出来ない)
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);


	SizeF renderSize = ArrangeOverride(arrangeRect.GetSize());
	m_finalLocalRect.x = finalLocalRect.x + m_margin.Left + arrangeRect.x;
	m_finalLocalRect.y = finalLocalRect.y + m_margin.Top + arrangeRect.y;
	m_finalLocalRect.width = renderSize.width;
	m_finalLocalRect.height = renderSize.height;

	OnLayoutUpdated();
}

//------------------------------------------------------------------------------
SizeF UIElement::MeasureOverride(const SizeF& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	SizeF size;
	// NaN の場合、この要素として必要な最小サイズは 0 となる。
	size.width = Math::IsNaNOrInf(m_size.width) ? 0.0f : m_size.width;
	size.height = Math::IsNaNOrInf(m_size.height) ? 0.0f : m_size.height;
	size.width = std::min(size.width, constraint.width);
	size.height = std::min(size.height, constraint.height);

	return size;
}

//------------------------------------------------------------------------------
SizeF UIElement::ArrangeOverride(const SizeF& finalSize)
{
	return finalSize;
}

//------------------------------------------------------------------------------
void UIElement::OnLayoutUpdated()
{
}

//------------------------------------------------------------------------------
void UIElement::OnRender(DrawingContext* g)
{
	if (m_background != nullptr)
	{
		g->SetBrush(m_background);
		g->DrawRectangle(m_finalLocalRect);
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
void UIElement::SetParent(UIElement* parent, UILayoutView* ownerLayoutView)
{
	m_parent = parent;
	m_ownerLayoutView = ownerLayoutView;
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
void UIElement::ActivateInternal(UIElement* child)
{
	if (m_parent != NULL) { m_parent->ActivateInternal(this); }
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
		UIStyle* style = styleTable->FindStyle(m_elementName);
		if (style != nullptr)
		{
			m_currentVisualStateStyle = style->FindStylePropertyTable(m_currentVisualStateName);
			m_invalidateFlags &= ~detail::InvalidateFlags::VisualState;
		}
		else
		{
			m_currentVisualStateStyle = nullptr;
		}
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
	localStyle->Apply(this);
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
void UIElement::UpdateLayout()
{
	// TODO: ここは GetRootStyleTable を使うべき？
	// 今は UILayoutView::UpdateLayout() からしか呼ばれていないので問題ないが…。
	ApplyTemplateHierarchy(m_ownerLayoutView->GetOwnerContext()->GetRootStyleTable(), nullptr);

	SizeF size(
		Math::IsNaNOrInf(m_size.width) ? m_ownerLayoutView->GetViewPixelSize().width : m_size.width,
		Math::IsNaNOrInf(m_size.height) ? m_ownerLayoutView->GetViewPixelSize().height : m_size.height);

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
	if (m_parent != NULL)
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
}

//------------------------------------------------------------------------------
void UIElement::Render(DrawingContext* g)
{
	//g->DrawRectangle(m_finalGlobalRect, Color(255,0,0,200));

	Matrix mat;
	mat.Translate(m_finalGlobalRect.x, m_finalGlobalRect.y, 0);
	g->SetTransform(mat);
	OnRender(g);

	// 子要素
	UIHelper::ForEachVisualChildren(this, [g](UIElement* child) { child->Render(g); });
}

//------------------------------------------------------------------------------
void UIElement::RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e)
{
	LN_CHECK_ARG(ev != NULL);
	LN_CHECK_ARG(e != NULL);
	tr::TypeInfo* thisType = tr::TypeInfo::GetTypeInfo(this);

	// this に AddHandler されているイベントハンドラを呼び出す。
	thisType->InvokeReflectionEvent(this, ev, e);

	// bubble
	if (!e->handled && m_parent != NULL)
	{
		m_parent->RaiseEventInternal(ev, e);
	}
}

LN_NAMESPACE_END
