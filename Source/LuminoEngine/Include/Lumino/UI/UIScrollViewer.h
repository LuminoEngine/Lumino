
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN


/**
	@brief	マウスドラッグが関係するイベント引数です。
*/
class UIDragDeltaEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIDragDeltaEventArgs() = default;
	virtual ~UIDragDeltaEventArgs() = default;

public:
	float horizontalChange;		/**< ドラッグ開始点からの水平距離 */
	float verticalChange;		/**< ドラッグ開始点からの垂直距離 */
};

/**
	@brief		
*/
class UIThumb
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_ROUTED_EVENT(UIDragDeltaEventArgs, DragStartedEvent);
	LN_ROUTED_EVENT(UIDragDeltaEventArgs, DragDeltaEvent);
	LN_ROUTED_EVENT(UIDragDeltaEventArgs, DragCompletedEvent);
	LN_ROUTED_EVENT(UIDragDeltaEventArgs, DragCanceledEvent);
	UIEvent<UIDragDeltaEventArgs>		DragStarted;		/**< マウスドラッグを開始したときに発生します。*/
	UIEvent<UIDragDeltaEventArgs>		DragDelta;			/**< マウスドラッグ中に発生します。*/
	UIEvent<UIDragDeltaEventArgs>		DragCompleted;		/**< マウスドラッグが完了したときに発生します。*/
	UIEvent<UIDragDeltaEventArgs>		DragCanceled;		/**< マウスドラッグを中断したときに発生します。*/

public:
	static RefPtr<UIThumb> Create();

protected:
	UIThumb();
	virtual ~UIThumb();
	void Initialize(detail::UIManager* manager);

	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void OnDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragStartedEvent, this, e); } }
	virtual void OnDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragDeltaEvent, this, e); } }
	virtual void OnDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCompletedEvent, this, e); } }
	virtual void OnDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCanceledEvent, this, e); } }

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;
};

/**
	@brief		
*/
class UITrack
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UITrack> Create();

protected:
	UITrack();
	virtual ~UITrack();
	void Initialize(detail::UIManager* manager);

};

/**
	@brief		
*/
class UIScrollBar
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIScrollBar> Create();

protected:
	UIScrollBar();
	virtual ~UIScrollBar();
	void Initialize(detail::UIManager* manager);

};

LN_NAMESPACE_END
