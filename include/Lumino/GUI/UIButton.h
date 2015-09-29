
#pragma once
#include "ContentControl.h"
#include "ButtonChrome.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief
	@details
				| VisualState | VisualStateGroup | description 
				|-------------|------------------|-------------
				| Normal      | CommonStates     | 既定の状態です。 
				| MouseOver   | CommonStates     | マウスカーソルがボタンの上にあります。
				| Pressed     | CommonStates     | ボタンが押されています。
				| Disabled    | CommonStates     | ボタンが無効状態です。
				| Focused     | FocusStates      | フォーカスがあります。
				| Unfocused   | FocusStates      | フォーカスがありません。
*/
class UIButton
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(UIButton);
public:
	static const String	IsMouseOverProperty;	///< IsMouseOver プロパティの識別子


	static const String ControlTemplateTypeName;	///< "Button"


	static UIButton* Create();

	UIButton(GUIManagerImpl* manager);
	virtual ~UIButton();

public:
	bool IsMouseOver() const { return m_isMouseOver; }

	//virtual void Render();

protected:
	virtual void OnClick();
	virtual void OnRender(Painter* painter);
	//virtual void Render();


	//virtual void OnMouseMove(MouseEventArgs* e);
	//void UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e);

private:
	void Handler_IsEnabledChanged(PropertyChangedEventArgs* e);
	void RoutedHandler_MouseEnter(MouseEventArgs* e);
	void RoutedHandler_MouseLeave(MouseEventArgs* e);
	void RoutedHandler_MouseMove(MouseEventArgs* e);
	void RoutedHandler_MouseDown(MouseEventArgs* e);
	void RoutedHandler_MouseUp(MouseEventArgs* e);

	// override UIElement

	//virtual bool ApplyTemplateInternal()
	//{
	//	if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
	//	return ContentControl::ApplyTemplateInternal();	// base
	//}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: 仮。ちゃんとテーマから読みだす
	bool m_isMouseOver;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
