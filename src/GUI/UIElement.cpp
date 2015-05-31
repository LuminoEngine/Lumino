
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElement
//=============================================================================

const String UIElement::SizeProperty(_T("Size"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
{
	LN_SAFE_ADDREF(m_manager);
	RegisterProperty(SizeProperty, SizeF(NAN, NAN));
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
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
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
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	const SizeF& size = GetSize();
	m_desiredSize.Width = std::min(size.Width, availableSize.Width);
	m_desiredSize.Height = std::min(size.Height, availableSize.Height);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalRect)
{
	m_finalRect = finalRect;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// サイズが定まっていない場合はレイアウトを決定できない
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	OnRender();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate(CombinedLocalResource* parent)
{
	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parent, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parent);
	}
	OnApplyTemplate();
}

//=============================================================================
// Decorator
//=============================================================================

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
	m_child = element;

	// 子要素のテーマを直ちに更新
	if (m_child != NULL) {
		m_child->ApplyTemplate(m_combinedLocalResource);
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
void Decorator::ApplyTemplate(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplate(parent);
	if (m_child != NULL) {
		m_child->ApplyTemplate(m_combinedLocalResource);	// 再帰的に更新する
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::AddChild(const Variant& value)
{
	UIElement* e = dynamic_cast<UIElement*>(value.GetObject());
	SetChild(e);

}

//=============================================================================
// ButtonChrome
//=============================================================================

const String ButtonChrome::TypeName(_T("ButtonChrome"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_bgMargin(2)
{
	//printf("TODO:");
	//m_brush.Attach(LN_NEW Graphics::TextureBrush());
	//m_brush->Create(_T("D:/Proj/Lumino/src/GUI/Resource/001-Blue01.png"), m_manager->GetGraphicsManager());	// TODO
	//m_brush->SetSourceRect(Rect(128, 0, 64, 64));

	//m_bgBrush.Attach(LN_NEW Graphics::TextureBrush());
	//m_bgBrush->SetTexture(m_brush->GetTexture());
	//m_bgBrush->SetSourceRect(Rect(0, 0, 128, 128));
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
void ButtonChrome::OnApplyTemplate()
{
	m_brush = static_cast<Graphics::TextureBrush*>(m_combinedLocalResource->GetItem(_T("ButtonNormalFrameBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnRender()
{
	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO

	RectF rect = m_finalRect;


	//painter.SetBrush(m_bgBrush);
	//painter.DrawRectangle(rect);

	painter.SetBrush(m_brush);
	painter.DrawFrameRectangle(rect, 8);
}

//=============================================================================
// ContentPresenter
//=============================================================================

const String ContentPresenter::TypeName(_T("ContentPresenter"));

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

//=============================================================================
// Control
//=============================================================================

const String Control::ControlTemplateTypeName(_T("Control"));

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::ApplyTemplate(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplate(parent);
	ApplyTemplateInternal();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Control::ApplyTemplateInternal()
{
	return ApplyTemplateInternalMain(ControlTemplateTypeName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Control::ApplyTemplateInternalMain(const String& typeName)
{
	ControlTemplate* t;
	if (m_combinedLocalResource->TryGetControlTemplate(typeName, &t))
	{
		t->Apply(this);
		return true;
	}
	return false;
}


//=============================================================================
// ContentControl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
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

	if (m_content.GetType() == VariantType_Object)
	{
		m_childElement = dynamic_cast<UIElement*>(m_content.GetObject());
	}

	// 子要素のテーマを直ちに更新
	if (m_childElement != NULL && m_combinedLocalResource != NULL) {
		m_childElement->ApplyTemplate(m_combinedLocalResource);
	}
}

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
void ContentControl::ApplyTemplate(CombinedLocalResource* parent)
{
	Control::ApplyTemplate(parent);
	if (m_childElement != NULL) {
		m_childElement->ApplyTemplate(m_combinedLocalResource);	// 再帰的に更新する
	}
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
		m_childElement->Render();
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
	return false;
}

//=============================================================================
// RootPane
//=============================================================================
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

const String Button::ControlTemplateTypeName(_T("Button"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManager* manager)
	: ContentControl(manager)
{


	//m_chrome.Attach(LN_NEW ButtonChrome(manager));
	//SetContent(Variant(m_chrome));
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


	//ContentControl::OnEvent(type, args)

	//if (m_childElement != NULL) {
	//	if (m_childElement->OnEvent(type, args)) { return true; }
	//}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnRender()
{
}

} // namespace GUI
} // namespace Lumino
