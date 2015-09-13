
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Thumb.h"
#include "Track.h"
#include "Panel.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/// Scroll イベントの原因となった動作を表します。
LN_ENUM(ScrollEventType)
{
	ThumbTrack = 0,		///< つまみがドラッグされている
	EndScroll,			///< つまみのドラッグが終了した
	SmallDecrement,		///< スクロールバーのデクリメントボタンがクリックされた
	SmallIncrement,		///< スクロールバーのインクリメントボタンがクリックされた
	LargeDecrement,		///< スクロールバーの PageUp 領域がクリックされた
	LargeIncrement,		///< スクロールバーの PageDown 領域がクリックされた
};
LN_ENUM_DECLARE(ScrollEventType);

/**
	@brief		ScrollBar のスクロールイベントの引数です。
*/
class ScrollEventArgs
	: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	ScrollEventArgs(float newValue, ScrollEventType type) { NewValue = newValue; Type = type; }
	virtual ~ScrollEventArgs() {}

public:
	float			NewValue;		///< 新しい値
	ScrollEventType	Type;			///< スクロールイベントの原因
};

/**
	@brief
*/
class ScrollBar
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollBar);
public:
	static const String PART_TrackKeyName;

	LN_PROPERTY(float,					ValueProperty);
	LN_PROPERTY(Orientation,			OrientationProperty);

	LN_ROUTED_EVENT(ScrollEventArgs,	ScrollEvent);


	// TODO: RoutedCommand は ID ではなく Command のポインタを直接公開した方が良いかも。
	// button->SetCommand(Window.CloseCommand) とかしたい。

public:
	static ScrollBarPtr Create();

	ScrollBar(GUIManager* manager);
	virtual ~ScrollBar();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/// ScrollBar の方向を指定します。
// TODO:	void SetOrientation(Orientation orientation) { m_orientation = orientation; OnPropertyChanged(OrientationProperty->GetName(), m_orientation); }

	/// ScrollBar の方向を取得します。規定値は Orientation::Horizontal です。
	Orientation GetOrientation() const { return m_orientation; }

	// TODO:↓後で RangeBase に移すかも

	/// スクロール位置に対する値を設定します。
	void SetValue(float value) { m_value = value; m_track->SetValue(value); }	// TODO: TemplateBinding のほうがよいか？

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

	RoutedEventSlot<ScrollEventArgs>	Scroll;
	
	///// ScrollEvent にハンドラを登録します。
	//void AddScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	///// ScrollEvent に登録されたハンドラを解除します。
	//void RemoveScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */


	void SetTrackExtent(float value) { m_track->SetMaximum(value); }
	void SetTrackViewportSize(float value) { m_track->SetViewportSize(value); }




protected:
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	void UpdateValue(float horizontalDragDelta, float verticalDragDelta);
	void ChangeValue(float newValue/*, bool defer*/);
	void Handler_Thumb_DragStarted(DragEventArgs* e);
	void Handler_Thumb_DragDelta(DragEventArgs* e);

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	//Event01<ScrollEventArgs*>	m_scrollEvent;

	Track*				m_track;	///< VisualTree 内の Track

	float				m_dragStartValue;	///< ドラッグ開始時の m_value
	bool				m_isStandalone;		///< Value の操作を this で行うか、ScrollViewer 経由で行うか。

	friend class ScrollViewer;
};

	
LN_NAMESPACE_GUI_END
} // namespace Lumino
