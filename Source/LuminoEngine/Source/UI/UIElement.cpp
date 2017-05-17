
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
// UIVisualStates
//==============================================================================
const String UIVisualStates::CommonGroup = _T("CommonGroup");
const String UIVisualStates::FocusGroup = _T("FocusGroup");
//const String UIVisualStates::ValidationStates = _T("ValidationStates");
const String UIVisualStates::NormalState = _T("Normal");
const String UIVisualStates::MouseOverState = _T("MouseOver");
const String UIVisualStates::PressedState = _T("Pressed");
const String UIVisualStates::DisabledState = _T("Disabled");
const String UIVisualStates::UnfocusedState = _T("Unfocused");
const String UIVisualStates::FocusedState = _T("Focused");
const String UIVisualStates::ValidState = _T("Valid");
const String UIVisualStates::InvalidState = _T("Invalid");

const String UIVisualStates::OrientationGroup = _T("OrientationGroup");
const String UIVisualStates::HorizontalState = _T("Horizontal");
const String UIVisualStates::VerticalState = _T("Vertical");

//==============================================================================
// UIElement
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIElement, tr::ReflectionObject);

// Property definition
LN_TR_PROPERTY_IMPLEMENT(UIElement, PointF, position, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, float, width, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, float, height, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, ThicknessF, margin, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, ThicknessF, padding, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, AlignmentAnchor, anchor, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, HAlignment, hAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, VAlignment, vAlignment, tr::PropertyMetadata());
//LN_TR_PROPERTY_IMPLEMENT(UIElement, BrushPtr, background, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, BrushPtr, foreground, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, BrushPtr, decoratorBackground, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIElement, float, decoratorOpacity, tr::PropertyMetadata());

// Event definition
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseEnterEvent, "MouseEnter", MouseEnter);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseLeaveEvent, "MouseLeave", MouseLeave);
//LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
//LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
//LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyDownEvent, "KeyDown", KeyDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyUpEvent, "KeyUp", KeyUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, TextInputEvent, "TextInput", TextInput);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIEventArgs, GotFocusEvent, "GotFocus", GotFocus);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIEventArgs, LostFocusEvent, "LostFocus", LostFocus);

//------------------------------------------------------------------------------
UIElement::UIElement()
	: m_manager(nullptr)
	, m_minSize(0, 0)
	, m_maxSize(Math::Inf, Math::Inf)
	, m_logicalParent(nullptr)
	, m_localStyle(nullptr)
	, m_currentVisualStateStyle(nullptr)
	, m_visualParent(nullptr)
	, m_visualChildren(nullptr)
	, position(PointF(0, 0))
	, width(NAN)
	, height(NAN)
	, margin(ThicknessF(0, 0, 0, 0))
	, padding(ThicknessF(0, 0, 0, 0))
	, anchor(AlignmentAnchor::None)
	, hAlignment(HAlignment::Stretch)
	, vAlignment(VAlignment::Stretch)
	, m_opacity(1.0f)
	, decoratorBackground(nullptr)
	, decoratorOpacity(1.0f)
	, m_combinedOpacity(0.0f)
	, m_isEnabled(true)
	, m_isMouseOver(nullptr)
	, m_isHitTestVisible(true)
	, m_isFocusable(false)
	, m_hasFocus(false)
{
}

//------------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_localStyle);
}

//------------------------------------------------------------------------------
void UIElement::Initialize()
{
	m_manager = detail::EngineDomain::GetUIManager();
	m_invalidateFlags |= detail::InvalidateFlags::Initializing;

	// 要素名を覚えておく。末端のサブクラスの名前となる。
	m_elementName = tr::TypeInfo::GetTypeInfo(this)->GetName();

	m_localStyle = LN_NEW UIStylePropertyTable();

	//GoToVisualState(String::GetEmpty());
	m_invalidateFlags |= detail::InvalidateFlags::VisualState;
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
void UIElement::SetBackground(Brush* value)
{
	m_localStyle->background = value;
	//tr::PropertyInfo::SetPropertyValueDirect<BrushPtr>(this, backgroundId, value);
}

//------------------------------------------------------------------------------
Brush* UIElement::GetBackground() const
{
	return m_localStyle->background.Get();
	//return tr::PropertyInfo::GetPropertyValueDirect<BrushPtr>(this, backgroundId);
}

//------------------------------------------------------------------------------
EventConnection UIElement::ConnectOnGotFocus(UIEventHandler handler)
{
	return m_onGotFocus.Connect(handler);
}

//------------------------------------------------------------------------------
EventConnection UIElement::ConnectOnLostFocus(UIEventHandler handler)
{
	return m_onLostFocus.Connect(handler);
}

//------------------------------------------------------------------------------
void UIElement::GoToVisualState(const StringRef& stateName)
{
	GetVisualStateManager()->GoToVisualState(stateName);
	//m_currentVisualStateName = stateName;
	m_invalidateFlags |= detail::InvalidateFlags::VisualState;
}

//------------------------------------------------------------------------------
UIVisualStateManager* UIElement::GetVisualStateManager()
{
	if (m_visualStateManager == nullptr)
	{
		m_visualStateManager = NewObject<UIVisualStateManager>();
	}
	return m_visualStateManager;
}

//------------------------------------------------------------------------------
void UIElement::Focus()
{
	if (IsFocusable())
	{
		GetContext()->SetFocusElement(this);
	}
}

//------------------------------------------------------------------------------
void UIElement::CaptureMouse()
{
	GetContext()->CaptureMouse(this);
}

//------------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	GetContext()->ReleaseMouseCapture(this);
}

//------------------------------------------------------------------------------
int UIElement::GetVisualChildrenCount() const
{
	if (m_visualChildren != nullptr)
	{
		return m_visualChildren->GetCount();
	}

	return 0;
}

//------------------------------------------------------------------------------
ILayoutElement* UIElement::GetVisualChild(int index) const
{
	if (m_visualChildren != nullptr)
	{
		return m_visualChildren->GetAt(index);
	}

	LN_THROW(0, InvalidOperationException);
	return nullptr;
}

//------------------------------------------------------------------------------
void UIElement::RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e)
{
	//if (m_parent != nullptr)
	{
		e->sender = sender;
		e->m_type = ev;
		RaiseEventInternal(e);
	}
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
void UIElement::ArrangeLayout(const Rect& finalLocalRect)
{
	const HAlignment* parentHAlign = (m_logicalParent != nullptr) ? m_logicalParent->GetLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (m_logicalParent != nullptr) ? m_logicalParent->GetLayoutContentVAlignment() : nullptr;

	Rect alignd = finalLocalRect;
	Size ds = GetLayoutDesiredSize();
	if (parentHAlign != nullptr)
	{
		detail::LayoutHelper::AdjustHorizontalAlignment(finalLocalRect.GetSize(), ds, true, *parentHAlign, &alignd);
		alignd.x += finalLocalRect.x;
	}
	if (parentVAlign != nullptr)
	{
		detail::LayoutHelper::AdjustVerticalAlignment(finalLocalRect.GetSize(), ds, true, *parentVAlign, &alignd);
		alignd.y += finalLocalRect.y;
	}


	ILayoutElement::ArrangeLayout(alignd/*finalLocalRect*/);

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
void UIElement::OnRender(DrawingContext* g)
{
	//g->SetBlendMode(BlendMode::Alpha);

	//if (background.Get() != nullptr)
	if (m_localStyle->background.Get() != nullptr)
	{
		g->SetBrush(m_localStyle->background.Get());
		//g->SetOpacity(m_combinedOpacity);
		//g->DrawRectangle(Rect(0, 0, m_finalLocalRect.GetSize()));
		g->DrawBoxBackground(Rect(0, 0, m_finalLocalRect.GetSize()), CornerRadius());

		
	}
	if (decoratorBackground.Get() != nullptr)
	{
		//g->SetBrush(decoratorBackground.Get());
		////g->SetOpacity(m_combinedOpacity * m_decoratorOpacity);
		//g->DrawRectangle(Rect(0, 0, m_finalLocalRect.GetSize()));
	}

	if (!m_localStyle->borderThickness.Get().IsZero())
	{
		g->DrawBoxBorder(
			Rect(0, 0, m_finalGlobalRect.GetSize()), m_localStyle->borderThickness.Get(), CornerRadius(),
			Color::Gray, Color::Gray, Color::Gray, Color::Gray,
			BorderDirection::Outside);
	}
}

//------------------------------------------------------------------------------
void UIElement::OnMouseMove(UIMouseEventArgs* e) { }
void UIElement::OnMouseDown(UIMouseEventArgs* e)
{
	if (IsFocusable())
	{
		Focus();

		// フォーカスを持ている要素はここで処理済みとする。そうしないと、ルート要素までルーティングされてしまう。
		e->handled = true;
	}
}
void UIElement::OnMouseUp(UIMouseEventArgs* e) { }
void UIElement::OnKeyDown(UIKeyEventArgs* e) { }
void UIElement::OnKeyUp(UIKeyEventArgs* e) { }
void UIElement::OnTextInput(UIKeyEventArgs* e) { }
void UIElement::OnGotFocus(UIEventArgs* e)
{
	m_onGotFocus.Raise(e);
}
void UIElement::OnLostFocus(UIEventArgs* e)
{
	m_onLostFocus.Raise(e);
}

//------------------------------------------------------------------------------
void UIElement::OnMouseEnter(UIMouseEventArgs* e)
{
	// 親にもマウスがはじめて乗ったのであれば親にも通知する
	if (m_visualParent != nullptr && !m_visualParent->m_isMouseOver)
	{
		m_visualParent->OnMouseEnter(e);
	}

	m_isMouseOver = true;

	//if (!e->handled) { RaiseEvent(MouseEnterEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::OnMouseLeave(UIMouseEventArgs* e)
{
	// 親にもマウスが乗ったことになっていれば、ヒットテストをした上で通知する
	if (m_visualParent != nullptr && m_visualParent->m_isMouseOver)
	{
		if (!m_visualParent->m_finalGlobalRect.Contains(e->GetPosition()))
		{
			m_visualParent->OnMouseLeave(e);
		}
	}

	m_isMouseOver = false;

	//if (!e->handled) { RaiseEvent(MouseLeaveEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::SetLogicalParent(UIElement* parent)
{
	if (parent != nullptr)
	{
		// 既に親があるとき、新しい親をつけることはできない
		LN_THROW(GetLogicalParent() == nullptr, InvalidOperationException, "the child elements of already other elements.");
	}

	m_logicalParent = parent;
}

//------------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	int count = GetVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		UIElement* e = static_cast<UIElement*>(GetVisualChild(i))->CheckMouseHoverElement(globalPt);
		if (e != nullptr) return e;
	}

	if (m_isHitTestVisible)
	{
		if (m_finalGlobalRect.Contains(globalPt)) {
			return this;
		}
	}

	return nullptr;
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
		if (m_isEnabled) RaiseEvent(UIEvents::MouseMoveEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseButtonDown:
		if (m_isEnabled) RaiseEvent(UIEvents::MouseDownEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseButtonUp:
		if (m_isEnabled) RaiseEvent(UIEvents::MouseUpEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseWheel:
		if (m_isEnabled) RaiseEvent(UIEvents::MouseWheelEvent, this, args);
		break;
	case detail::UIInternalEventType::KeyDown:
		if (m_isEnabled) RaiseEvent(KeyDownEvent, this, args);
		break;
	case detail::UIInternalEventType::KeyUp:
		if (m_isEnabled) RaiseEvent(KeyUpEvent, this, args);
		break;
	case detail::UIInternalEventType::TextInput:
		if (m_isEnabled) RaiseEvent(TextInputEvent, this, args);
		break;
	case detail::UIInternalEventType::ElapsedTime:
		break;
	case detail::UIInternalEventType::MouseEnter:	// TODO: 親領域
		if (m_isEnabled) RaiseEvent(MouseEnterEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseLeave:	// TODO: 親領域
		if (m_isEnabled) RaiseEvent(MouseLeaveEvent, this, args);
		break;
	default:
		break; 
	}

	return args->handled;
}

//------------------------------------------------------------------------------
// this でハンドリングしたいルーティングイベントが発生したとき、イベント経由ではなく
// 通常の仮想関数で通知することで、パフォーマンスの向上を図る。
// (UI要素作成時のイベントハンドラの new や AddHandler をする必要がなくなる)
void UIElement::OnRoutedEvent(UIEventArgs* e)
{
	auto ev = e->GetType();
	if (ev == UIEvents::MouseMoveEvent)
	{
		OnMouseMove(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseDownEvent)
	{
		OnMouseDown(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseUpEvent)
	{
		OnMouseUp(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == KeyDownEvent)
	{
		OnKeyDown(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == KeyUpEvent)
	{
		OnKeyUp(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == TextInputEvent)
	{
		OnTextInput(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == MouseEnterEvent)
	{
		OnMouseEnter(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == MouseLeaveEvent)
	{
		OnMouseLeave(static_cast<UIMouseEventArgs*>(e));
	}
}

//------------------------------------------------------------------------------
void UIElement::CallOnGotFocus()
{
	LN_ASSERT(!m_hasFocus);
	m_hasFocus = true;
	OnGotFocus(UIEventArgs::Create(GotFocusEvent, this));
}

//------------------------------------------------------------------------------
void UIElement::CallOnLostFocus()
{
	LN_ASSERT(m_hasFocus);
	m_hasFocus = false;
	OnLostFocus(UIEventArgs::Create(LostFocusEvent, this));
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

	// Style 更新
	UpdateLocalStyleAndApplyProperties(styleTable, parentStyle);

	// 子要素
	UIStylePropertyTable* localStyle = m_localStyle;
	UIHelper::ForEachVisualChildren(this, [styleTable, localStyle](UIElement* child) { child->ApplyTemplateHierarchy(styleTable, localStyle); });
}

//------------------------------------------------------------------------------
void UIElement::UpdateLocalStyleAndApplyProperties(UIStyleTable* styleTable, UIStylePropertyTable* parentStyle)
{
	if (LN_CHECK_STATE(m_localStyle != nullptr)) return;


	// TODO: styleTable は多分 Context のルート固定でよい。

	// parent → state の順で local へマージする
	// TODO: このへんのコピーが時間かかりそうならリビジョンカウント使うとか対策する。毎フレームやってるから多分重い。
	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;
	if (parentStyle != nullptr)       invalidate |= m_localStyle->UpdateInherit(parentStyle);
	//if (currentStateStyle != nullptr) invalidate |= m_localStyle->UpdateInherit(currentStateStyle);



	// VisualState の変更がある場合
	if (m_invalidateFlags.TestFlag(detail::InvalidateFlags::VisualState))
	{

		auto* vm = GetVisualStateManager();

		//String subStateName;
		//GetStyleClassName(&subStateName);


		UIStyle* style = styleTable->FindStyle(tr::TypeInfo::GetTypeInfo(this)/*, GetStyleSubControlName()*/);
		if (style != nullptr)
		{
			invalidate |= style->MergeActiveStylePropertyTables(m_localStyle, vm->GetActiveStateNames());

			//for (auto& stateName : vm->GetActiveStateNames())
			//{
			//	auto* st = style->FindStylePropertyTable(stateName);
			//	if (st != nullptr) invalidate |= m_localStyle->UpdateInherit(st);
			//}
			//m_currentVisualStateStyle = style->FindStylePropertyTable(m_currentVisualStateName);
		}
		//else
		//{
		//	m_currentVisualStateStyle = nullptr;
		//}

		style = styleTable->FindSubControlStyle(m_styleSubControlOwnerName, m_styleSubControlName);
		if (style != nullptr)
		{
			invalidate |= style->MergeActiveStylePropertyTables(m_localStyle, vm->GetActiveStateNames());
		}

		m_invalidateFlags &= ~detail::InvalidateFlags::VisualState;
	}


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
UIContext* UIElement::GetContext() const
{
	return UIContext::GetMainContext();// TODO
}

//------------------------------------------------------------------------------
void UIElement::UpdateLayout(const Size& viewSize)
{
	OnUpdatingLayout();
	ILayoutElement::UpdateLayout(viewSize);
}

//------------------------------------------------------------------------------
detail::SpcialUIElementType UIElement::GetSpcialUIElementType() const
{
	return detail::SpcialUIElementType::Common;
}

//------------------------------------------------------------------------------
void UIElement::UpdateTransformHierarchy(const Rect& parentGlobalRect)
{
	if (m_visualParent != nullptr)
	{
		//m_finalGlobalRect.x = m_parent->m_finalGlobalRect.x + m_finalLocalRect.x;
		//m_finalGlobalRect.y = m_parent->m_finalGlobalRect.y + m_finalLocalRect.y;
		m_combinedOpacity = m_visualParent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
	}
	else
	{
		//m_finalGlobalRect.x = m_finalLocalRect.x;
		//m_finalGlobalRect.y = m_finalLocalRect.y;
		m_combinedOpacity = m_opacity;
	}
	//m_finalGlobalRect.width = m_finalLocalRect.width;
	//m_finalGlobalRect.height = m_finalLocalRect.height;

	//// 子要素
	//UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });

	ILayoutElement::UpdateTransformHierarchy(parentGlobalRect);

	// 最初の更新おわり
	m_invalidateFlags &= ~detail::InvalidateFlags::Initializing;
	m_invalidateFlags &= ~detail::InvalidateFlags::ParentChangedUpdating;
}

//------------------------------------------------------------------------------
void UIElement::Render(DrawingContext* g)
{
	//g->Clear(ClearFlags::Color, Color::White);

	Matrix mat;
	mat.Translate(m_finalGlobalRect.x, m_finalGlobalRect.y, 0);
	g->SetTransform(mat);



	//g->DrawBoxBorder(Rect(50, 50, 300, 200), ThicknessF(10, 10, 10, 10), Color::Red, Color::Green, Color::Blue, Color::Cyan, 10, 10, 10, 10);	// TODO:
	//g->DrawBoxShadow(Rect(10, 20, 300, 400), Color::Black, 5, 5, false);
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
//void UIElement::GetStyleClassName(String* outSubStateName)
//{
//	*outSubStateName = String::GetEmpty();
//}

//------------------------------------------------------------------------------
void UIElement::RaiseEventInternal(UIEventArgs* e)
{
	if (LN_CHECK_ARG(e != nullptr)) return;

	// まずは this に通知
	OnRoutedEvent(e);
	if (e->handled) return;

	// this に AddHandler されているイベントハンドラを呼び出す
	tr::TypeInfo* thisType = tr::TypeInfo::GetTypeInfo(this);
	thisType->InvokeReflectionEvent(this, e->GetType(), e);
	if (e->handled) return;

	// bubble
	if (m_visualParent != nullptr)
	{
		m_visualParent->RaiseEventInternal(e);
	}
}

//------------------------------------------------------------------------------
void UIElement::AddVisualChild(UIElement* element)
{
	if (LN_CHECK_ARG(element != nullptr)) return;
	if (LN_CHECK_ARG(element->m_visualParent == nullptr)) return;

	// リストが作成されていなければ、ここで始めて作る (省メモリ)
	if (m_visualChildren == nullptr)
	{
		m_visualChildren = std::make_shared<List<RefPtr<UIElement>>>();
	}

	m_visualChildren->Add(element);
	element->m_visualParent = this;
}

//------------------------------------------------------------------------------
void UIElement::RemoveVisualChild(UIElement* element)
{
	//LN_FAIL_CHECK_ARG(element != nullptr) return;
	if (element == nullptr) return;
	if (m_visualChildren == nullptr) return;

	m_visualChildren->Remove(element);
	element->m_visualParent = nullptr;
}

//------------------------------------------------------------------------------
const PointF& UIElement::GetLayoutPosition() const { return position; }
Size UIElement::GetLayoutSize() const { return Size(width, height); }
const ThicknessF& UIElement::GetLayoutMargin() const { return margin; }
const ThicknessF& UIElement::GetLayoutPadding() const { return padding; }
AlignmentAnchor UIElement::GetLayoutAnchor() const { return anchor; }
HAlignment UIElement::GetLayoutHAlignment() const { return hAlignment; }
VAlignment UIElement::GetLayoutVAlignment() const { return vAlignment; }
void UIElement::GetLayoutMinMaxInfo(Size* outMin, Size* outMax) const { *outMin = m_minSize; *outMax = m_maxSize; }
ILayoutElement* UIElement::GetLayoutParent() const { return m_visualParent; }
const HAlignment* UIElement::GetLayoutContentHAlignment() { return GetPriorityContentHAlignment(); }
const VAlignment* UIElement::GetLayoutContentVAlignment() { return GetPriorityContentVAlignment(); }
const Size& UIElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void UIElement::SetLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void UIElement::SetLayoutFinalLocalRect(const Rect& rect) { m_finalLocalRect = rect; }
const Rect& UIElement::GetLayoutFinalLocalRect() const { return m_finalLocalRect; }
void UIElement::SetLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }

LN_NAMESPACE_END
