#pragma once
#include "UIEvents.hpp"
#include "UIControl.hpp"

namespace ln {

class UIScrollView
	: public UIControl
{
public:
	UIScrollView();
	void init();

private:
};

/**
 * ユーザー操作によりドラッグイベントを発生させる UI 要素です。
 *
 * ドラッグ開始ポイントからどれだけの距離がドラッグされたかを通知しますが、位置の変更やドロップ機能は提供しません。
 */
class UIThumb
	: public UIElement
{
LN_CONSTRUCT_ACCESS:
	UIThumb();
	virtual ~UIThumb();
	void init();

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void onDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(UIEvents::ScrollDragStartedEvent, this, e); } }
	virtual void onDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(UIEvents::ScrollDragDeltaEvent, this, e); } }
	virtual void onDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(UIEvents::ScrollDragCompletedEvent, this, e); } }
	virtual void onDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(UIEvents::ScrollDragCanceledEvent, this, e); } }

private:
	Point	m_lastScreenPosition;
	bool	m_isDragging;

	//Event<UIDragDeltaEventArgs>        DragStarted;
	//Event<UIDragDeltaEventArgs>        DragDelta;
	//Event<UIDragDeltaEventArgs>        DragCompleted;
	//Event<UIDragDeltaEventArgs>        DragCanceled;

};

} // namespace ln

