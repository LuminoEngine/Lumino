
#pragma once
#include "ContentControl.h"

LN_NAMESPACE_BEGIN
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
	LN_ROUTED_EVENT(RoutedEventArgs,		ClickEvent);	/**< Click ルーティングイベントの識別子 */

public:
	//-------------------------------------------------------------------------
	/** @name RoutedEvents */
	/** @{ */

	RoutedEventSlot<RoutedEventArgs>		Click;			/**< ボタンがクリックされたときに発生します。*/

	/** @{ */






	static const String ControlTemplateTypeName;	///< "Button"

	
	/**
		@brief		StackPanel を作成します。
		@param[in]	parent	: この要素の親要素
		@param[in]	content	: 表示するコンテンツ
		@return		作成された StackPanel。このオブジェクトは AutoReleasePool で管理されています。
	*/
	static UIButton* Create(UIElement* parent, const Variant& content);

	UIButton(GUIManagerImpl* manager);
	virtual ~UIButton();

public:
	//bool IsMouseOver() const { return m_isMouseOver; }

	//virtual void Render();

protected:
	virtual void OnClick();
	virtual void OnRender(RenderingContext* painter);
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

private:
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
