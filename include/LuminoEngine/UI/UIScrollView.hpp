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
	virtual const String& className() const  override { static String name = u"UIThumb"; return name; }
	virtual void onRoutedEvent(UIEventArgs* e) override;

	virtual void onDragStarted(UIDragDeltaEventArgs* e);
	virtual void onDragDelta(UIDragDeltaEventArgs* e);
	virtual void onDragCompleted(UIDragDeltaEventArgs* e);
	virtual void onDragCanceled(UIDragDeltaEventArgs* e);

private:
	Point	m_lastScreenPosition;
	bool	m_isDragging;

	//Event<UIDragDeltaEventArgs>        DragStarted;
	//Event<UIDragDeltaEventArgs>        DragDelta;
	//Event<UIDragDeltaEventArgs>        DragCompleted;
	//Event<UIDragDeltaEventArgs>        DragCanceled;

};

} // namespace ln

