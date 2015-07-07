
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/VisualState.h>	// for button

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElement
//=============================================================================

const PropertyID	UIElement::SizeProperty(_T("Size"));
const PropertyID	UIElement::HorizontalAlignmentProperty(_T("HorizontalAlignment"));
const PropertyID	UIElement::VerticalAlignmentProperty(_T("VerticalAlignment"));

const EventID	UIElement::MouseMoveEvent(_T("MouseMove"));
const EventID	UIElement::MouseLeaveEvent(_T("MouseLeave"));
const EventID	UIElement::MouseEnterEvent(_T("MouseEnter"));
const EventID	UIElement::MouseDownEvent(_T("MouseDown"));
const EventID	UIElement::MouseUpEvent(_T("MouseUp"));

const EventID	UIElement::CanExecuteRoutedCommandEvent(_T("CanExecuteRoutedCommandEvent"));
const EventID	UIElement::ExecuteRoutedCommandEvent(_T("ExecuteRoutedCommandEvent"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
	, m_parent(NULL)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
	, m_horizontalAlignment(HorizontalAlignment::Stretch)
	, m_verticalAlignment(VerticalAlignment::Stretch)
	, m_rootLogicalParent(NULL)
	, m_templateParent(NULL)
{
	LN_SAFE_ADDREF(m_manager);

	// プロパティの登録
	LN_DEFINE_PROPERTY(UIElement, SizeF, SizeProperty, &UIElement::SetSize, &UIElement::GetSize, SizeF());
	LN_DEFINE_PROPERTY_ENUM(UIElement, HorizontalAlignment, HorizontalAlignmentProperty, &UIElement::SetHorizontalAlignment, &UIElement::GetHorizontalAlignment, HorizontalAlignment::Stretch);
	LN_DEFINE_PROPERTY_ENUM(UIElement, VerticalAlignment, VerticalAlignmentProperty, &UIElement::SetVerticalAlignment, &UIElement::GetVerticalAlignment, VerticalAlignment::Stretch);
	//RegisterProperty(SizeProperty, SizeF(NAN, NAN));

	// イベントの登録
	LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent,	[](UIElement* target, MouseEventArgs* e) { target->MouseMove(e); });
	LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseDownEvent,	[](UIElement* target, MouseEventArgs* e) { target->MouseDown(e); });
	LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseUpEvent,		[](UIElement* target, MouseEventArgs* e) { target->MouseUp(e); });
	
	LN_DEFINE_ROUTED_EVENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent,	[](UIElement* target, CanExecuteRoutedCommandEventArgs* e) { target->Handler_CanExecuteRoutedCommandEvent(e); });
	LN_DEFINE_ROUTED_EVENT(UIElement, ExecuteRoutedCommandEventArgs,	ExecuteRoutedCommandEvent,		[](UIElement* target, ExecuteRoutedCommandEventArgs* e) { target->Handler_ExecuteRoutedCommandEvent(e); });

	// 削除予定
	//m_eventDataStore.Add(MouseMoveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseLeaveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseEnterEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_localResource);
	LN_SAFE_RELEASE(m_combinedLocalResource);
	LN_SAFE_RELEASE(m_manager);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::CaptureMouse() 
{
	m_manager->CaptureMouse(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	m_manager->ReleaseMouseCapture(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate()
{
	CombinedLocalResource* parentResource = NULL;
	if (m_parent != NULL) {
		parentResource = m_parent->m_combinedLocalResource;
	}
	else {
		parentResource = m_manager->GetRootCombinedResource();	// 親要素が無ければ Manager のリソースを使う
	}
	ApplyTemplateHierarchy(parentResource);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// 子要素を優先
	LN_FOREACH(UIElement* child, m_visualChildren) {
		UIElement* e = child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	//if (m_templateChild != NULL)
	//{
	//	UIElement* e = m_templateChild->CheckMouseHoverElement(globalPt);
	//	if (e != NULL) { return e; }
	//	// 子論理要素は UIElement の担当ではない。
	//	// ContetntControl 等でこの関数をオーバーライドし、そちらに実装する。
	//}

	if (m_finalRect.Contains(globalPt)) {
		return this;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// ① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm


	m_desiredSize = MeasureOverride(availableSize);

	//const SizeF& size = GetSize();
	//m_desiredSize.Width = std::min(size.Width, m_desiredSize.Width);
	//m_desiredSize.Height = std::min(size.Height, m_desiredSize.Height);

	// 子要素
	if (m_templateChild != NULL) {
		m_templateChild->MeasureLayout(m_desiredSize);
	}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->MeasureLayout(m_desiredSize);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalRect)
{
	// finalRect はこの要素を配置できる領域サイズ。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	SizeF renderSize = ArrangeOverride(finalRect.GetSize());
	m_finalRect.X = finalRect.X;
	m_finalRect.Y = finalRect.Y;
	m_finalRect.Width = renderSize.Width;
	m_finalRect.Height = renderSize.Height;

	// 子要素 (もし複数あれば m_finalRect の領域に重ねられるように配置される)
	if (m_templateChild != NULL) {
		m_templateChild->ArrangeLayout(m_finalRect);
	}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->ArrangeLayout(m_finalRect);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UIElement::MeasureOverride(const SizeF& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	SizeF size;
	size.Width = Math::IsNaNOrInf(m_size.Width) ? 0.0f : size.Width;
	size.Height = Math::IsNaNOrInf(m_size.Height) ? 0.0f : size.Height;
	size.Width = std::min(size.Width, constraint.Width);
	size.Height = std::min(size.Height, constraint.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateBinding(Property* thisProp, const String& srcPropPath, UIElement* rootLogicalParent)
{
	if (m_rootLogicalParent == NULL)
	{
		// TODO: 解除は？
		rootLogicalParent->PropertyChanged.AddHandler(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
		m_rootLogicalParent = rootLogicalParent;
	}
	else if (m_rootLogicalParent != rootLogicalParent) {
		// あってはならない。
		// VisualTree 要素が異なる ルート Logcal 要素にバインドしようとした。
		LN_THROW(0, InvalidOperationException);
	}

	TemplateBindingInfo info;
	info.ThisProp = thisProp;
	info.SourcePropPath = srcPropPath;
	m_templateBindingInfoList.Add(info);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	// 子要素
	//if (m_templateChild != NULL) {
	//	m_templateChild->Render();
	//}
	LN_FOREACH(UIElement* child, m_visualChildren) {
		child->Render();
	}

	OnRender();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(EventType type, EventArgs* args)
{
	// 子要素
	//if (m_templateChild != NULL) {
	//	if (m_templateChild->OnEvent(type, args)) { return true; }
	//}
	LN_FOREACH(UIElement* child, m_visualChildren) {
		if (child->OnEvent(type, args)) { return true; }
	}

	switch (type)
	{
	case Lumino::GUI::EventType_Unknown:
		break;
	case Lumino::GUI::EventType_MouseMove:
		OnMouseMove(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonDown:
		OnMouseDown(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonUp:
		OnMouseUp(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseWheel:
		break;
	case Lumino::GUI::EventType_KeyDown:
		break;
	case Lumino::GUI::EventType_KeyUp:
		break;
	case Lumino::GUI::EventType_ElapsedTime:
		break;
	case Lumino::GUI::EventType_MouseEnter:
		//RaiseEvent(MouseEnterEvent, this, args);
		break;
	case Lumino::GUI::EventType_MouseLeave:
		//RaiseEvent(MouseLeaveEvent, this, args);
		break;
	default:
		break;
	}

	//if (type == EventType_MouseMove) {
	//	//RaiseEvent<MouseEventArgs>(MouseMoveEvent, this, (MouseEventArgs*)args);
	//}
	return args->Handled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parent, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parent);
	}
	OnApplyTemplate(m_combinedLocalResource);

	// 子要素
	//if (m_templateChild != NULL) {
	//	m_templateChild->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	//}
	LN_FOREACH(UIElement* child, m_visualChildren) {
		child->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateChild(UIElement* child)
{
	//LN_VERIFY_RETURN(child != NULL);

	// 古い要素があればいろいろ解除する
	if (m_templateChild != NULL)
	{
		m_visualChildren.Remove(m_templateChild);
		// TODO: templateBinding 用の PropertyChanged がひつようかもしれない。
		PropertyChanged.Clear();
		//rootLogicalParent->PropertyChanged(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
	}
	if (child != NULL)
	{
		LN_VERIFY_RETURN(child->m_templateParent == NULL);
		RefPtr<UIElement> t(child, true);
		m_templateChild = child;
		m_visualChildren.Add(child);
		child->m_parent = this;
		child->m_templateParent = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIElement::AddVisualChild(UIElement* child)
//{
//	LN_VERIFY_RETURN(child != NULL);
//	LN_VERIFY_RETURN(child->m_parent == NULL);
//
//	RefPtr<UIElement> t(child, true);
//	m_visualChildren.Add(t);
//	child->m_parent = this;
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_CanExecuteRoutedCommandEvent(CanExecuteRoutedCommandEventArgs* e)
{
	// TODO: リバースイテレータでないと、コマンドのオーバーライドに対応できない
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			e->CanExecute = command->CanExecute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled が false なら親 UIElement の Handler_CanExecuteRoutedCommandEvent へ流れる。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_ExecuteRoutedCommandEvent(ExecuteRoutedCommandEventArgs* e)
{
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			command->Execute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled が false なら親 UIElement の Handler_ExecuteRoutedCommandEvent へ流れる。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e)
{
	LN_FOREACH(TemplateBindingInfo& info, m_templateBindingInfoList)
	{
		if (info.SourcePropPath == e->PropertyName) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
}

//=============================================================================
// Decorator
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(Decorator);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::Decorator(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::~Decorator()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::SetChild(UIElement* element)
{
	// 既に別の子要素となっている場合はエラー
	if (element != NULL && element->GetParent() != NULL) {
		LN_THROW(0, InvalidOperationException, _T("Specified element is already a child element of another element."));
	}

	m_child = element;

	if (m_child != NULL)
	{
		m_child->SetParent(this);
		m_child->ApplyTemplate();	// 子要素のテーマを直ちに更新
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::GetChild() const
{
	return m_child;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::AddChild(const Variant& value)
{
	UIElement* e = dynamic_cast<UIElement*>(value.GetObject());
	if (e == NULL) {
		LN_THROW(0, NotImplementedException);
	}
	SetChild(e);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_child != NULL) {	// 子要素を優先
		UIElement* e = m_child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Decorator::AddVisualChild(UIElement* child)
//{
//	// Decorator は LogicalTree = VisualTree なはずなのでこれでいいはず・・・
//	SetChild(child);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	if (m_child != NULL) {
		m_child->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Decorator::AddChildToVisualTree(UIElement* element)
//{
//
//}
#endif

//=============================================================================
// ButtonChrome
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonChrome);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonChrome);
const String	ButtonChrome::IsMouseOverProperty(_T("IsMouseOver"));
const String	ButtonChrome::FrameWidthProperty(_T("FrameWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8.0f)
	, m_isMouseOver(false)
{
	// プロパティの登録
	LN_DEFINE_PROPERTY(ButtonChrome, bool, IsMouseOverProperty, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver, false);

	// TODO
	// ボタンのスタイルとテンプレート
	// https://msdn.microsoft.com/ja-jp/library/ms753328%28v=vs.110%29.aspx
	RefPtr<VisualStateManager> vsm(LN_NEW VisualStateManager());

	RefPtr<VisualStateGroup> group1(LN_NEW VisualStateGroup(_T("CommonStates")));


	RefPtr<VisualState> state1(LN_NEW VisualState(_T("Normal")));

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::~ButtonChrome()
{
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::SetPropertyValue(const String& propertyName, const Variant& value)
{
	if (propertyName == FrameWidthProperty) { m_frameWidth = value.GetFloat(); return; }
	Decorator::SetPropertyValue(propertyName, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant ButtonChrome::GetPropertyValue(const String& propertyName) const
{
	if (propertyName == FrameWidthProperty) { return Variant(m_frameWidth); }
	return Decorator::GetPropertyValue(propertyName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_brush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalFrameBrush")));
	m_bgBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalBackgroundBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnRender()
{
	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO

	RectF bgRect = m_finalRect;
	RectF rect = m_finalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter.SetBrush(m_bgBrush);
		painter.DrawRectangle(bgRect);
	}

	painter.SetBrush(m_brush);
	painter.DrawFrameRectangle(rect, m_frameWidth);
}

//=============================================================================
// ContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::ContentPresenter(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::~ContentPresenter()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::SetContent(UIElement* content)
{
	m_content = content;
	m_visualChildren.Add(m_content);	// m_visualChildren に追加したものは OnEvent や Render が呼ばれるようになる
}

#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	if (m_content != NULL) {
		m_content->ApplyTemplateHierarchy(m_combinedLocalResource);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::MeasureOverride(const SizeF& constraint)
{
	if (m_content != NULL)
	{
		m_content->MeasureLayout(constraint);
		return m_content->GetDesiredSize();
	}
	return UIElement::MeasureOverride(constraint);

#if 0
	SizeF desiredSize = UIElement::MeasureOverride(constraint);
	if (m_content != NULL)
	{
		m_content->MeasureLayout(constraint);
		const SizeF& contentDesiredSize = m_content->GetDesiredSize();
		desiredSize.Width  = std::max(desiredSize.Width, contentDesiredSize.Width);
		desiredSize.Height = std::max(desiredSize.Height, contentDesiredSize.Height);
	}
	return desiredSize;
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::ArrangeOverride(const SizeF& finalSize)
{
	if (m_content != NULL) {
		m_content->ArrangeLayout(RectF(0, 0, finalSize));
	}
	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::Render()
{
	if (m_content != NULL) {
		m_content->Render();
	}
	UIElement::Render();
}

#endif

//=============================================================================
// ItemsPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::ItemsPresenter(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::~ItemsPresenter()
{

}

//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::~Control()
{

}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternal()
//{
//	return ApplyTemplateInternalMain(ControlTemplateTypeName);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternalMain(const String& typeName)
//{
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(typeName, &t))
//	{
//		t->Apply(this);
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	// ローカルリソースを更新する
	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parent, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parent);
	}

	// ControlTemplate の適用処理
	ControlTemplate* t;
	if (m_combinedLocalResource->TryGetControlTemplate(GetTypeID(), &t)) {
		t->Apply(this);
	}

	OnApplyTemplate(m_combinedLocalResource);

	// 子要素
	if (m_templateChild != NULL) {
		m_templateChild->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}


//=============================================================================
// ContentControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentControl);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
	, m_contentPresenter(NULL)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::~ContentControl()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::SetContent(Variant value)
{
	m_content = value;

	// Object 型であれば UIElement ととして取り出してみる
	if (m_content.GetType() == VariantType_Object)
	{
		UIElement* element = dynamic_cast<UIElement*>(m_content.GetObject());
		if (element != NULL) {
			m_childElement = element;
		}
	}

	m_content.SetFloat(0);

	if (m_childElement != NULL)
	{
		LN_THROW(m_childElement->GetParent() == NULL, InvalidOperationException);	// 既に親要素があった
		//m_childElement->SetParent(this);

		m_contentPresenter->SetContent(m_childElement);

		// 子要素のテーマを直ちに更新
		if (m_combinedLocalResource != NULL) {
			m_childElement->ApplyTemplate();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::PollingTemplateChildCreated(UIElement* element)
{
	ContentPresenter* presenter = dynamic_cast<ContentPresenter*>(element);
	if (presenter != NULL) {
		m_contentPresenter = presenter;
	}
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::Render()
{
	OnRender();
	if (m_childElement != NULL) {
		m_childElement->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* ContentControl::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_childElement != NULL) {	// 子要素を優先
		UIElement* e = m_childElement->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::MeasureLayout(const SizeF& availableSize)
{
	if (m_childElement != NULL) {
		m_childElement->MeasureLayout(availableSize);	// 特に枠とかないのでそのままのサイズを渡せる
	}
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ArrangeLayout(const RectF& finalRect)
{
	if (m_childElement != NULL) {
		m_childElement->ArrangeLayout(finalRect);	// 特に枠とかないのでそのままのサイズを渡せる
	}
	Control::ArrangeLayout(finalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::OnRender()
{
	if (m_childElement != NULL) {
		m_childElement->Render();	//TODO: Render からも呼んでる・・・
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ContentControl::OnEvent(EventType type, EventArgs* args)
{
	if (m_childElement != NULL) {
		if (m_childElement->OnEvent(type, args)) { return true; }
	}
	return Control::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	Control::ApplyTemplateHierarchy(parent);
	if (m_childElement != NULL) {
		m_childElement->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}
#endif

//=============================================================================
// RootPane
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(RootPane);
LN_UI_ELEMENT_SUBCLASS_IMPL(RootPane);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::RootPane(GUIManager* manager)
	: ContentControl(manager)
{
	//// デフォルトテーマ
	//m_combinedLocalResource = LN_NEW CombinedLocalResource();
	//m_combinedLocalResource->Combine(NULL, m_manager->GetDefaultTheme());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::~RootPane()
{

}

//=============================================================================
// Button
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Button);
LN_UI_ELEMENT_SUBCLASS_IMPL(Button);
const String	Button::IsMouseOverProperty(_T("IsMouseOver"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManager* manager)
	: ContentControl(manager)
	, m_isMouseOver(false)
{
	// プロパティの登録
	LN_DEFINE_PROPERTY(Button, bool, IsMouseOverProperty, NULL, &Button::IsMouseOver, false);

	//m_chrome.Attach(LN_NEW ButtonChrome(manager));
	//SetContent(Variant(m_chrome));

	//MouseMove += LN_CreateDelegate(this, &Button::UIElement_MouseMove);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::~Button()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnClick()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Button::OnEvent(EventType type, EventArgs* args)
{

	//

	//if (m_childElement != NULL) {
	//	if (m_childElement->OnEvent(type, args)) { return true; }
	//}
	return ContentControl::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnRender()
{
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnMouseMove(MouseEventArgs* e)
{
	m_isMouseOver = true;
	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e)
//{
//	m_isMouseOver = true;
//	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::Render()
//{
//	printf("Button::Render()\n");
//	ContentControl::Render();
//}

} // namespace GUI
} // namespace Lumino
