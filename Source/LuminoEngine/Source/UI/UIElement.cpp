
#include "Internal.h"
#include <math.h>
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIStyle.h>
#include "UIHelper.h"
#include "UIManager.h"

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
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIElement, RuntimeResource);

// Event definition
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIEventArgs, GotFocusEvent, "GotFocus", GotFocus);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIEventArgs, LostFocusEvent, "LostFocus", LostFocus);

//------------------------------------------------------------------------------
UIElement::UIElement()
	: m_manager(nullptr)
	, m_minSize(0, 0)
	, m_maxSize(Math::Inf, Math::Inf)
	, m_logicalParent(nullptr)
	, m_localStyle(nullptr)
	//, m_currentVisualStateStyle(nullptr)
	, m_visualParent(nullptr)
	, m_visualChildren(nullptr)
	, position(PointF(0, 0))
	, width(NAN)
	, height(NAN)
	, anchor(AlignmentAnchor::None)
	, hAlignment(HAlignment::Stretch)
	, vAlignment(VAlignment::Stretch)
	, decoratorBackground(nullptr)
	, decoratorOpacity(1.0f)
	, m_specialElementType(UISpecialElementType::None)
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
}

//------------------------------------------------------------------------------
void UIElement::initialize()
{
	RuntimeResource::initialize();
	m_manager = detail::EngineDomain::getUIManager();
	m_invalidateFlags |= detail::InvalidateFlags::Initializing;

	// 要素名を覚えておく。末端のサブクラスの名前となる。
	m_elementName = tr::TypeInfo::getTypeInfo(this)->getName();

	m_localStyle = Ref<detail::UIStylePropertyTableInstance>::makeRef();

	//goToVisualState(String::GetEmpty());
	m_invalidateFlags |= detail::InvalidateFlags::VisualState;
}

//------------------------------------------------------------------------------
void UIElement::setMargin(const ThicknessF& margin)
{
	m_localStyle->margin = margin;
}

//------------------------------------------------------------------------------
const ThicknessF& UIElement::getMargin() const
{
	return m_localStyle->margin;
}

//------------------------------------------------------------------------------
void UIElement::setPadding(const ThicknessF& Padding)
{
	m_localStyle->padding = Padding;
}

//------------------------------------------------------------------------------
const ThicknessF& UIElement::getPadding() const
{
	return m_localStyle->padding;
}

//------------------------------------------------------------------------------
void UIElement::setLayoutColumn(int index) { m_gridLayoutInfo.layoutColumn = index; }
int UIElement::getLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
void UIElement::setLayoutRow(int index) { m_gridLayoutInfo.layoutRow = index; }
int UIElement::getLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
void UIElement::setLayoutColumnSpan(int span) { m_gridLayoutInfo.layoutColumnSpan = span; }
int UIElement::getLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
void UIElement::setLayoutRowSpan(int span) { m_gridLayoutInfo.layoutRowSpan = span; }
int UIElement::getLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }

//------------------------------------------------------------------------------
void UIElement::setBackground(Brush* value)
{
	m_localStyle->background = value;
	//tr::PropertyInfo::setPropertyValueDirect<BrushPtr>(this, backgroundId, value);
}

//------------------------------------------------------------------------------
Brush* UIElement::getBackground() const
{
	return m_localStyle->background.get();
	//return tr::PropertyInfo::getPropertyValueDirect<BrushPtr>(this, backgroundId);
}

//------------------------------------------------------------------------------
EventConnection UIElement::connectOnGotFocus(UIEventHandler handler)
{
	return m_onGotFocus.connect(handler);
}

//------------------------------------------------------------------------------
EventConnection UIElement::connectOnLostFocus(UIEventHandler handler)
{
	return m_onLostFocus.connect(handler);
}

//------------------------------------------------------------------------------
void UIElement::goToVisualState(const StringRef& stateName)
{
	getVisualStateManager()->goToVisualState(stateName);
	//m_currentVisualStateName = stateName;
	m_invalidateFlags |= detail::InvalidateFlags::VisualState;
}

//------------------------------------------------------------------------------
UIVisualStateManager* UIElement::getVisualStateManager()
{
	if (m_visualStateManager == nullptr)
	{
		m_visualStateManager = newObject<UIVisualStateManager>();
	}
	return m_visualStateManager;
}

//------------------------------------------------------------------------------
void UIElement::focus()
{
	if (isFocusable())
	{
		getContext()->setFocusElement(this);
	}
}

//------------------------------------------------------------------------------
void UIElement::captureMouse()
{
	getManager()->captureMouse(this);
}

//------------------------------------------------------------------------------
void UIElement::releaseMouseCapture()
{
	getManager()->releaseMouseCapture(this);
}

//------------------------------------------------------------------------------
int UIElement::getVisualChildrenCount() const
{
	if (m_visualChildren != nullptr)
	{
		return m_visualChildren->getCount();
	}

	return 0;
}

//------------------------------------------------------------------------------
ILayoutElement* UIElement::getVisualChild(int index) const
{
	if (m_visualChildren != nullptr)
	{
		return m_visualChildren->getAt(index);
	}

	LN_THROW(0, InvalidOperationException);
	return nullptr;
}

//------------------------------------------------------------------------------
void UIElement::raiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e)
{
	//if (m_parent != nullptr)
	{
		e->sender = sender;
		e->m_type = ev;
		raiseEventInternal(e);
	}
}

//------------------------------------------------------------------------------
void UIElement::measureLayout(const Size& availableSize)
{
	ILayoutElement::measureLayout(availableSize);

	// フォントの無効フラグを落とす
	// TODO: UITextElement へ移動した方が良いかも？
	m_invalidateFlags &= ~detail::InvalidateFlags::Font;
}

//------------------------------------------------------------------------------
void UIElement::arrangeLayout(const Rect& finalLocalRect)
{
	const HAlignment* parentHAlign = (m_logicalParent != nullptr) ? m_logicalParent->getLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (m_logicalParent != nullptr) ? m_logicalParent->getLayoutContentVAlignment() : nullptr;

	Rect alignd = finalLocalRect;
	Size ds = getLayoutDesiredSize();
	if (parentHAlign != nullptr)
	{
		detail::LayoutHelper::adjustHorizontalAlignment(finalLocalRect.getSize(), ds, true, *parentHAlign, &alignd);
		alignd.x += finalLocalRect.x;
	}
	if (parentVAlign != nullptr)
	{
		detail::LayoutHelper::adjustVerticalAlignment(finalLocalRect.getSize(), ds, true, *parentVAlign, &alignd);
		alignd.y += finalLocalRect.y;
	}


	ILayoutElement::arrangeLayout(alignd/*finalLocalRect*/);

	onLayoutUpdated();
}

//------------------------------------------------------------------------------
Size UIElement::measureOverride(const Size& constraint)
{
	return ILayoutElement::measureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UIElement::arrangeOverride(const Size& finalSize)
{
	return ILayoutElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UIElement::onUpdateFrame()
{
}

//------------------------------------------------------------------------------
void UIElement::onLayoutUpdated()
{
}

//------------------------------------------------------------------------------
void UIElement::onRender(DrawingContext* g)
{
	//g->setBlendMode(BlendMode::Alpha);
	//g->setDepthTestEnabled(false);
	//g->setDepthWriteEnabled(false);

	//if (background.Get() != nullptr)
	if (m_localStyle->background.get() != nullptr)
	{
		g->setBrush(m_localStyle->background.get());
		//g->drawRectangle(Rect(0, 0, m_finalLocalRect.getSize()));
		g->drawBoxBackground(Rect(0, 0, m_finalLocalRenderRect.getSize()), m_localStyle->cornerRadius.get());
	}
	//else
	//{
	//	g->setBrush(SolidColorBrush::create(Color(0.5, 0.5, 0.5, 0.5)));
	//	g->drawRectangle(Rect(0, 0, m_finalLocalRect.getSize()));
	//	//g->drawBoxBackground(Rect(0, 0, m_finalLocalRect.getSize()), m_localStyle->cornerRadius.get());
	//}
	if (decoratorBackground.get() != nullptr)
	{
		//g->setBrush(decoratorBackground.Get());
		////g->setOpacity(m_combinedOpacity * m_decoratorOpacity);
		//g->drawRectangle(Rect(0, 0, m_finalLocalRect.GetSize()));
	}

	//if (m_localStyle->testDeco.get() != nullptr)
	//{
	//	m_localStyle->testDeco.get()->layoutAndRender(g, m_finalGlobalRect.GetSize());
	//}
	for (auto& re : m_localStyle->m_availableRenderElements)
	{
		re->layoutAndRender(g, m_finalGlobalRect.getSize());
	}

	if (!m_localStyle->borderThickness.get().isZero())
	{
		g->drawBoxBorder(
			Rect(0, 0, m_finalGlobalRect.getSize()), m_localStyle->borderThickness.get(), m_localStyle->cornerRadius.get(),
			Color::Gray, Color::Gray, Color::Gray, Color::Gray,
			BorderDirection::Inside);
	}
}

//------------------------------------------------------------------------------
void UIElement::onMouseMove(UIMouseEventArgs* e) { }
void UIElement::onMouseDown(UIMouseEventArgs* e)
{
	if (isFocusable())
	{
		focus();

		// フォーカスを持ている要素はここで処理済みとする。そうしないと、ルート要素までルーティングされてしまう。
		e->handled = true;
	}
}
void UIElement::onMouseUp(UIMouseEventArgs* e) { }
void UIElement::onKeyDown(UIKeyEventArgs* e) { }
void UIElement::onKeyUp(UIKeyEventArgs* e) { }
void UIElement::onTextInput(UIKeyEventArgs* e) { }
void UIElement::onGotFocus(UIEventArgs* e)
{
	m_onGotFocus.raise(e);
}
void UIElement::onLostFocus(UIEventArgs* e)
{
	m_onLostFocus.raise(e);
}

//------------------------------------------------------------------------------
void UIElement::onMouseEnter(UIMouseEventArgs* e)
{
	// 親にもマウスがはじめて乗ったのであれば親にも通知する
	if (m_visualParent != nullptr && !m_visualParent->m_isMouseOver)
	{
		m_visualParent->onMouseEnter(e);
	}

	m_isMouseOver = true;

	//if (!e->handled) { raiseEvent(MouseEnterEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::onMouseLeave(UIMouseEventArgs* e)
{
	// 親にもマウスが乗ったことになっていれば、ヒットテストをした上で通知する
	if (m_visualParent != nullptr && m_visualParent->m_isMouseOver)
	{
		if (!m_visualParent->m_finalGlobalRect.contains(e->getPosition()))
		{
			m_visualParent->onMouseLeave(e);
		}
	}

	m_isMouseOver = false;

	//if (!e->handled) { raiseEvent(MouseLeaveEvent, this, e); }
}

//------------------------------------------------------------------------------
void UIElement::setLogicalParent(UIElement* parent)
{
	if (parent != nullptr)
	{
		// 既に親があるとき、新しい親をつけることはできない
		LN_THROW(getLogicalParent() == nullptr, InvalidOperationException, "the child elements of already other elements.");
	}

	m_logicalParent = parent;
}

//------------------------------------------------------------------------------
UIElement* UIElement::checkMouseHoverElement(const PointF& globalPt)
{
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	int count = getVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		UIElement* e = static_cast<UIElement*>(getVisualChild(i))->checkMouseHoverElement(globalPt);
		if (e != nullptr) return e;
	}

	if (m_isHitTestVisible)
	{
		PointF localPoint = globalPt;
		if (m_visualParent != nullptr)
		{
			localPoint.x -= m_visualParent->m_finalGlobalRect.x;
			localPoint.y -= m_visualParent->m_finalGlobalRect.y;
		}

		if (onHitTest(localPoint)) {
			return this;
		}
	}

	return nullptr;
}

//------------------------------------------------------------------------------
bool UIElement::onEvent(detail::UIInternalEventType type, UIEventArgs* args)
{
	/* 今のところ、イベントを再帰で通知していく必要はない。
	マウスイベントは Hover しているものへ Manager が直接送り込む。
	キーイベントはフォーカスを持っているものへ Manager が直接送り込む。

	…というか、再帰で通知してはならない。マウスイベントとかは再帰してしまうと、
	マウスカーソルが乗っていない要素がイベントを受け取ってしまうことになる。
	*/
	// 後ろからループする。後のモノが上に描画されるので、この方が自然。
	// TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
	//int count = getVisualChildrenCount();
	//for (int i = count - 1; i >= 0; i--)
	//{
	//	if (getVisualChild(i)->onEvent(type, args)) { return true; }
	//}


	switch (type)
	{
	case detail::UIInternalEventType::Unknown:
		break;
	case detail::UIInternalEventType::MouseMove:
		if (m_isEnabled) raiseEvent(UIEvents::MouseMoveEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseButtonDown:
		if (m_isEnabled) raiseEvent(UIEvents::MouseDownEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseButtonUp:
		if (m_isEnabled) raiseEvent(UIEvents::MouseUpEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseWheel:
		if (m_isEnabled) raiseEvent(UIEvents::MouseWheelEvent, this, args);
		break;
	case detail::UIInternalEventType::KeyDown:
		if (m_isEnabled) raiseEvent(UIEvents::KeyDownEvent, this, args);
		break;
	case detail::UIInternalEventType::KeyUp:
		if (m_isEnabled) raiseEvent(UIEvents::KeyUpEvent, this, args);
		break;
	case detail::UIInternalEventType::TextInput:
		if (m_isEnabled) raiseEvent(UIEvents::TextInputEvent, this, args);
		break;
	case detail::UIInternalEventType::ElapsedTime:
		break;
	case detail::UIInternalEventType::MouseEnter:	// TODO: 親領域
		if (m_isEnabled) raiseEvent(UIEvents::MouseEnterEvent, this, args);
		break;
	case detail::UIInternalEventType::MouseLeave:	// TODO: 親領域
		if (m_isEnabled) raiseEvent(UIEvents::MouseLeaveEvent, this, args);
		break;
	default:
		break; 
	}

	return args->handled;
}

//------------------------------------------------------------------------------
// this でハンドリングしたいルーティングイベントが発生したとき、イベント経由ではなく
// 通常の仮想関数で通知することで、パフォーマンスの向上を図る。
// (UI要素作成時のイベントハンドラの new や addHandler をする必要がなくなる)
void UIElement::onRoutedEvent(UIEventArgs* e)
{
	auto ev = e->getType();
	if (ev == UIEvents::MouseMoveEvent)
	{
		onMouseMove(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseDownEvent)
	{
		onMouseDown(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseUpEvent)
	{
		onMouseUp(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::KeyDownEvent)
	{
		onKeyDown(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == UIEvents::KeyUpEvent)
	{
		onKeyUp(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == UIEvents::TextInputEvent)
	{
		onTextInput(static_cast<UIKeyEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseEnterEvent)
	{
		onMouseEnter(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIEvents::MouseLeaveEvent)
	{
		onMouseLeave(static_cast<UIMouseEventArgs*>(e));
	}
}

//------------------------------------------------------------------------------
void UIElement::callOnGotFocus()
{
	LN_ASSERT(!m_hasFocus);
	m_hasFocus = true;
	onGotFocus(UIEventArgs::create(GotFocusEvent, this));
}

//------------------------------------------------------------------------------
void UIElement::callOnLostFocus()
{
	LN_ASSERT(m_hasFocus);
	m_hasFocus = false;
	onLostFocus(UIEventArgs::create(LostFocusEvent, this));
}

//------------------------------------------------------------------------------
//void UIElement::ApplyTemplate()
//{
//	/// 現在のテンプレートからビジュアルツリーを再構築します。
//	/// この関数は必要なタイミングでレイアウトシステムから呼び出されます。通常、明示的に呼び出す必要はありません。
//	///		というか、呼び出しちゃダメ。必ずルートから再帰的に更新しないと、もし親がまだ ApplyTemplate() してない状態でこれを呼ぶと
//	///		ローカルリソースが正しく更新されない。
//	///		TODO: もしかしたら、setParent した瞬間にローカルリソースを更新したほうが良いかも？
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
//	applyTemplateHierarchy();
//}

//------------------------------------------------------------------------------
void UIElement::applyTemplateHierarchy(UIStyleTable* styleTable, detail::UIStylePropertyTableInstance* parentStyle)
{

	// Style 更新
	updateLocalStyleAndApplyProperties(styleTable, parentStyle);

	// 子要素
	detail::UIStylePropertyTableInstance* localStyle = m_localStyle;
	UIHelper::forEachVisualChildren(this, [styleTable, localStyle](UIElement* child) { child->applyTemplateHierarchy(styleTable, localStyle); });
}

//------------------------------------------------------------------------------
void UIElement::updateLocalStyleAndApplyProperties(UIStyleTable* styleTable, detail::UIStylePropertyTableInstance* parentStyleInstance)
{
	if (LN_CHECK_STATE(m_localStyle != nullptr)) return;


	// TODO: styleTable は多分 Context のルート固定でよい。

	// parent → state の順で local へマージする
	// TODO: このへんのコピーが時間かかりそうならリビジョンカウント使うとか対策する。毎フレームやってるから多分重い。
	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;
	//if (parentStyleInstance != nullptr)       invalidate |= m_localStyle->inheritParentElementStyle(parentStyleInstance);



	// VisualState の変更がある場合
	if (m_invalidateFlags.TestFlag(detail::InvalidateFlags::VisualState))
	{
		m_localStyle->clearAvailableRenderElements();
		m_localStyle->readyMergeProcess();

		auto* vm = getVisualStateManager();

		UIStyle* style = styleTable->findSubControlStyle(m_styleSubControlOwnerName, m_styleSubControlName);
		if (style != nullptr)
		{
			invalidate |= style->mergeActiveStylePropertyTables(m_localStyle, vm->getActiveStateNames());
		}
		else
		{
			style = styleTable->findStyle(tr::TypeInfo::getTypeInfo(this)/*, GetStyleSubControlName()*/);
			if (style != nullptr)
			{
				invalidate |= style->mergeActiveStylePropertyTables(m_localStyle, vm->getActiveStateNames());

			}
		}



		m_invalidateFlags &= ~detail::InvalidateFlags::VisualState;
	}


	if (invalidate != detail::InvalidateFlags::None)
	{
		onUpdateStyle(m_localStyle, invalidate);
	}

}

//------------------------------------------------------------------------------
void UIElement::onUpdateStyle(detail::UIStylePropertyTableInstance* localStyle, detail::InvalidateFlags invalidateFlags)
{
	localStyle->apply(this, !m_invalidateFlags.TestFlag(detail::InvalidateFlags::Initializing));
	// TODO: UITextElement::onUpdateStyle 参照
	// TODO: アニメーション
}

//------------------------------------------------------------------------------
void UIElement::onUpdatingLayout()
{
	// 子要素
	UIHelper::forEachVisualChildren(this, [](UIElement* child) { child->onUpdatingLayout(); });
}

//------------------------------------------------------------------------------
bool UIElement::onHitTest(const PointF& localPoint)
{
	return m_finalLocalRenderRect.contains(localPoint);
}

//------------------------------------------------------------------------------
UIContext* UIElement::getContext() const
{
	return UIContext::getMainContext();// TODO
}

//------------------------------------------------------------------------------
const ThicknessF& UIElement::getMargineInternal() const { return m_localStyle->margin.get(); }

//------------------------------------------------------------------------------
void UIElement::updateLayout(const Size& viewSize)
{
	onUpdatingLayout();
	ILayoutElement::updateLayout(viewSize);
}

//------------------------------------------------------------------------------
detail::SpcialUIElementType UIElement::getSpcialUIElementType() const
{
	return detail::SpcialUIElementType::Common;
}

//------------------------------------------------------------------------------
void UIElement::updateTransformHierarchy(const Rect& parentGlobalRect)
{

	//// 子要素
	//UIHelper::forEachVisualChildren(this, [](UIElement* child) { child->updateTransformHierarchy(); });

	ILayoutElement::updateTransformHierarchy(parentGlobalRect);

	// 最初の更新おわり
	m_invalidateFlags &= ~detail::InvalidateFlags::Initializing;
	m_invalidateFlags &= ~detail::InvalidateFlags::ParentChangedUpdating;
}

//------------------------------------------------------------------------------
void UIElement::updateFrame()
{
	onUpdateFrame();

	// call children
	UIHelper::forEachVisualChildren(this, [](UIElement* child) { child->updateFrame(); });
}

//------------------------------------------------------------------------------
void UIElement::render(DrawingContext* g)
{
	//PointF contentOffset;
	if (m_visualParent != nullptr)
	{
		detail::BuiltinEffectData::combine(m_visualParent->m_combinedBuiltinEffectData, m_builtinEffectData, &m_combinedBuiltinEffectData);
		//contentOffset = m_visualParent->m_finalLocalContentRect.getTopLeft();
	}
	else
	{
		m_combinedBuiltinEffectData = m_builtinEffectData;
	}

	g->pushState();

	Matrix mat;
	mat.translate(m_finalGlobalRect.x, m_finalGlobalRect.y, 0);
	g->setTransform(mat);
	g->setBuiltinEffectData(m_combinedBuiltinEffectData);

	

	//g->drawBoxBorder(Rect(50, 50, 300, 200), ThicknessF(10, 10, 10, 10), Color::Red, Color::Green, Color::Blue, Color::Cyan, 10, 10, 10, 10);	// TODO:
	//g->drawBoxShadow(Rect(10, 20, 300, 400), Color::Black, 5, 5, false);
	onRender(g);

	// 子要素
	UIHelper::forEachVisualChildren(this, [g](UIElement* child) { child->render(g); });

	g->popState();	// TODO: scoped
}

//------------------------------------------------------------------------------
const HAlignment* UIElement::getPriorityContentHAlignment()
{
	return nullptr;
}

//------------------------------------------------------------------------------
const VAlignment* UIElement::getPriorityContentVAlignment()
{
	return nullptr;
}

//------------------------------------------------------------------------------
//void UIElement::GetStyleClassName(String* outSubStateName)
//{
//	*outSubStateName = String::GetEmpty();
//}

//------------------------------------------------------------------------------
void UIElement::raiseEventInternal(UIEventArgs* e)
{
	if (LN_CHECK_ARG(e != nullptr)) return;

	// まずは this に通知
	onRoutedEvent(e);
	if (e->handled) return;

	// this に addHandler されているイベントハンドラを呼び出す
	tr::TypeInfo* thisType = tr::TypeInfo::getTypeInfo(this);
	thisType->invokeReflectionEvent(this, e->getType(), e);
	if (e->handled) return;

	// bubble
	if (m_visualParent != nullptr)
	{
		m_visualParent->raiseEventInternal(e);
	}
}

//------------------------------------------------------------------------------
void UIElement::addVisualChild(UIElement* element)
{
	if (LN_CHECK_ARG(element != nullptr)) return;
	if (LN_CHECK_ARG(element->m_visualParent == nullptr)) return;

	// リストが作成されていなければ、ここで始めて作る (省メモリ)
	if (m_visualChildren == nullptr)
	{
		m_visualChildren = std::make_shared<List<Ref<UIElement>>>();
	}

	m_visualChildren->add(element);
	element->m_visualParent = this;
}

//------------------------------------------------------------------------------
void UIElement::removeVisualChild(UIElement* element)
{
	//LN_FAIL_CHECK_ARG(element != nullptr) return;
	if (element == nullptr) return;
	if (m_visualChildren == nullptr) return;

	m_visualChildren->remove(element);
	element->m_visualParent = nullptr;
}

//------------------------------------------------------------------------------
const PointF& UIElement::getLayoutPosition() const { return position; }
Size UIElement::getLayoutSize() const { return Size(width, height); }
const ThicknessF& UIElement::getLayoutMargin() const { return m_localStyle->margin.get(); }
const ThicknessF& UIElement::getLayoutPadding() const { return m_localStyle->padding.get(); }
AlignmentAnchor UIElement::getLayoutAnchor() const { return anchor; }
HAlignment UIElement::getLayoutHAlignment() const { return hAlignment; }
VAlignment UIElement::getLayoutVAlignment() const { return vAlignment; }
void UIElement::getLayoutMinMaxInfo(Size* outMin, Size* outMax) const { *outMin = m_minSize; *outMax = m_maxSize; }
ILayoutElement* UIElement::getLayoutParent() const { return m_visualParent; }
const HAlignment* UIElement::getLayoutContentHAlignment() { return getPriorityContentHAlignment(); }
const VAlignment* UIElement::getLayoutContentVAlignment() { return getPriorityContentVAlignment(); }
const Size& UIElement::getLayoutDesiredSize() const { return m_desiredSize; }
void UIElement::setLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void UIElement::setLayoutFinalLocalRect(const Rect& renderRect, const Rect& contentRect) { m_finalLocalRenderRect = renderRect; m_finalLocalContentRect = contentRect; }
void UIElement::getLayoutFinalLocalRect(Rect* outRenderRect, Rect* outContentRect) const { *outRenderRect = m_finalLocalRenderRect; *outContentRect = m_finalLocalContentRect; }
void UIElement::setLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }

LN_NAMESPACE_END
