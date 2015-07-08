
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollBar
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollBar);
public:
	static const EventID	ScrollEvent;

	// TODO: RoutedCommand は ID ではなく Command のポインタを直接公開した方が良いかも。
	// button->SetCommand(Window.CloseCommand) とかしたい。

public:
	ScrollBar(GUIManager* manager);
	virtual ~ScrollBar();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/// ScrollBar の方向を指定します。
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// ScrollBar の方向を取得します。規定値は Orientation::Horizontal です。
	Orientation GetOrientation() const { return m_orientation; }

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	
	/// ScrollEvent にハンドラを登録します。
	void AddScrollEventHandler(Delegate02<CoreObject*, ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	/// ScrollEvent に登録されたハンドラを解除します。
	void RemoveScrollEventHandler(Delegate02<CoreObject*, ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */

protected:
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	Orientation	m_orientation;
	Event02<CoreObject*, ScrollEventArgs*>	m_scrollEvent;

};


} // namespace GUI
} // namespace Lumino
