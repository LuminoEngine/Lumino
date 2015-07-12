
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

	// TODO:↓後で RangeBase に移すかも

	/// スクロール位置に対する値を設定します。
	void SetValue(float value) { m_value = value; }

	/// スクロール位置に対する値を取得します。規定値は 0 です。
	float GetValue() const { return m_value; }

	/// 指定可能な最小値を設定します。
	void SetMinimum(float value) { m_minimum = value; }

	/// 指定可能な最小値を取得します。規定値は 0 です。
	float GetMinimum() const { return m_minimum; }

	/// 指定可能な最大値を設定します。
	void SetMaximum(float value) { m_maximum = value; }

	/// 指定可能な最大値を取得します。規定値は 1 です。
	float GetMaximum() const { return m_maximum; }

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	
	/// ScrollEvent にハンドラを登録します。
	void AddScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	/// ScrollEvent に登録されたハンドラを解除します。
	void RemoveScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */

protected:
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	Event01<ScrollEventArgs*>	m_scrollEvent;

};


} // namespace GUI
} // namespace Lumino
