
#include "../Internal.h"
#include <Lumino/GUI/ButtonChrome.h>
#include <Lumino/GUI/VisualState.h>	// for button

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ButtonChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonChrome, Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonChrome);

// Register property
LN_DEFINE_PROPERTY_2(ButtonChrome, bool, IsMouseOverProperty, "IsMouseOver", 0.0f, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver);
LN_DEFINE_PROPERTY_2(ButtonChrome, float, FrameWidthProperty, "FrameWidth", 8.0f, &ButtonChrome::SetFrameWidth, &ButtonChrome::GetFrameWidth);

//const String	ButtonChrome::IsMouseOverProperty(_T("IsMouseOver"));
//const String	ButtonChrome::FrameWidthProperty(_T("FrameWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome* ButtonChrome::Create(GUIManager* manager)
{
	auto obj = RefPtr<ButtonChrome>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8.0f)
	, m_isMouseOver(false)
{
	// プロパティの登録
	//LN_DEFINE_PROPERTY(ButtonChrome, bool, IsMouseOverProperty, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver, false);

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
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void ButtonChrome::SetPropertyValue(const Property* prop, const Variant& value)
//{
//	if (prop == FrameWidthProperty) { m_frameWidth = value.GetFloat(); return; }
//	Decorator::SetPropertyValue(prop, value);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//Variant ButtonChrome::GetPropertyValue(const Property* prop) const
//{
//	if (prop == FrameWidthProperty) { return Variant(m_frameWidth); }
//	return Decorator::GetPropertyValue(prop);
//}

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
void ButtonChrome::OnRender(Graphics::Painter* painter)
{
	RectF bgRect = m_finalGlobalRect;
	RectF rect = m_finalGlobalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter->SetBrush(m_bgBrush);
		painter->DrawRectangle(bgRect);
	}

	painter->SetBrush(m_brush);
	painter->DrawFrameRectangle(rect, m_frameWidth);
}

} // namespace GUI
} // namespace Lumino
