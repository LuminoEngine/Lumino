
#pragma once
#include "UIElement.h"
#include "UIButton.h"
#include "UIItemsControl.h"

LN_NAMESPACE_BEGIN
class IScrollInfo;
class UIScrollEventArgs;
using UIScrollEventArgsPtr = RefPtr<UIScrollEventArgs>;

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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragStartedEvent);		/**< マウスドラッグを開始したときに発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragDeltaEvent);			/**< マウスドラッグ中に発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCompletedEvent);		/**< マウスドラッグが完了したときに発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCanceledEvent);		/**< マウスドラッグを中断したときに発生するイベントを表します。*/

public:
	static RefPtr<UIThumb> create();

LN_CONSTRUCT_ACCESS:
	UIThumb();
	virtual ~UIThumb();
	void initialize();

protected:
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual void OnDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragStartedEventId, this, e); } }
	virtual void OnDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragDeltaEventId, this, e); } }
	virtual void OnDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCompletedEventId, this, e); } }
	virtual void OnDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCanceledEventId, this, e); } }

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;

	UIEvent<UIDragDeltaEventArgs>        DragStarted;
	UIEvent<UIDragDeltaEventArgs>        DragDelta;
	UIEvent<UIDragDeltaEventArgs>        DragCompleted;
	UIEvent<UIDragDeltaEventArgs>        DragCanceled;
};

/**
	@brief		
*/
class UITrack
	: public UIElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	// TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
	static const String OrientationStates;
	static const String HorizontalState;
	static const String VerticalState;


	static RefPtr<UITrack> create();
public:

	/** Track の方向を指定します。*/
	void SetOrientation(Orientation orientation);

	/** Track の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation GetOrientation() const { return m_orientation; }

	/** スクロール位置に対する値を設定します。*/
	void SetValue(float value) { m_value = value; }

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float getValue() const { return m_value; }

	/** 指定可能な最小値を設定します。*/
	void SetMinimum(float value) { m_minimum = value; }

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float GetMinimum() const { return m_minimum; }

	/** 指定可能な最大値を設定します。*/
	void SetMaximum(float value) { m_maximum = value; }

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float GetMaximum() const { return m_maximum; }

	/** スクロール可能なコンテンツの中で表示される部分のサイズを設定します。*/
	void SetViewportSize(float value) { m_viewportSize = value; }

	/** スクロール可能なコンテンツの中で表示される部分のサイズを取得します。*/
	float GetViewportSize() const { return m_viewportSize; }

	/** 距離を値に変換します。*/
	float ValueFromDistance(float horizontal, float vertical);

	UIThumb* GetThumb() const;
	UIButton* GetDecreaseButton() const;
	UIButton* GetIncreaseButton() const;

LN_CONSTRUCT_ACCESS:

	UITrack();
	virtual ~UITrack();
	void initialize();

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	void CoerceLength(float& componentLength, float trackLength);
	void CalcSliderComponentsSize(
		const Size& arrangeSize,
		bool isVertical,
		float* outDecreaseButtonLength,
		float* outThumbLength,
		float* outIncreaseButtonLength);
	void CalcScrollBarComponentsSize(
		float trackLength,
		float viewportSize,
		float* outDecreaseButtonLength,
		float* outThumbLength,
		float* outIncreaseButtonLength);

	Orientation			m_orientation;
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	float				m_density;
	float				m_viewportSize;
	RefPtr<UIButton>	m_decreaseButton;
	RefPtr<UIThumb>		m_thumb;
	RefPtr<UIButton>	m_increaseButton;

};

/** Scroll イベントの原因となった動作を表します。*/
enum class ScrollEventType
{
	ThumbTrack,			/** つまみがドラッグされている */
	EndScroll,			/** つまみのドラッグが終了した */
	SmallDecrement,		/** スクロールバーのデクリメントボタンがクリックされた */
	SmallIncrement,		/** スクロールバーのインクリメントボタンがクリックされた */
	LargeDecrement,		/** スクロールバーの PageUp 領域がクリックされた */
	LargeIncrement,		/** スクロールバーの PageDown 領域がクリックされた */
};

/**
	@brief        ScrollBar のスクロールイベントの引数です。
*/
class UIScrollEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static UIScrollEventArgsPtr create(Object* sender, float newValue, ScrollEventType type, bool caching = true);

	UIScrollEventArgs();
	virtual ~UIScrollEventArgs();
	void initialize(Object* sender, float newValue_, ScrollEventType type_);

public:
	float			newValue;	/** 新しい値 */
	ScrollEventType	type;		/** スクロールイベントの原因 */
};

/**
	@brief		
*/
class UIScrollBar
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_ROUTED_EVENT2(UIScrollEventArgs, ScrollEvent);

	// TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
	static const String OrientationStates;
	static const String HorizontalState;
	static const String VerticalState;




	static RefPtr<UIScrollBar> create();
	// TODO:↓後で RangeBase に移すかも


	/** ScrollBar の方向を指定します。*/
	void SetOrientation(Orientation orientation);

	/** ScrollBar の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation GetOrientation() const;

	/** スクロール位置に対する値を設定します。*/
	void SetValue(float value);

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float getValue() const;

	/** 指定可能な最小値を設定します。*/
	void SetMinimum(float value);

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float GetMinimum() const;

	/** 指定可能な最大値を設定します。*/
	void SetMaximum(float value);

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float GetMaximum() const;

	/** スクロール可能なコンテンツの中で表示される部分のサイズを設定します。*/
	void SetViewportSize(float value);

	/** スクロール可能なコンテンツの中で表示される部分のサイズを取得します。*/
	float GetViewportSize() const;

LN_CONSTRUCT_ACCESS:
	UIScrollBar();
	virtual ~UIScrollBar();
	void initialize();

protected:
	// UIElement interface
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	//virtual void GetStyleClassName(String* outSubStateName);

private:
	void UpdateValue(float horizontalDragDelta, float verticalDragDelta);

	RefPtr<UITrack>		m_track;
	RefPtr<UIButton>	m_lineUpButton;
	RefPtr<UIButton>	m_lineDownButton;
	float				m_dragStartValue;
};

/**
	@brief
*/
class UIScrollViewer
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	static RefPtr<UIScrollViewer> create();

LN_CONSTRUCT_ACCESS:
	UIScrollViewer();
	virtual ~UIScrollViewer();
	void initialize();

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRoutedEvent(UIEventArgs* e) override;

	// UIControl interface
	virtual void OnLayoutPanelChanged(UILayoutPanel* newPanel) override;

private:
	RefPtr<UIScrollBar>			m_horizontalScrollBar;
	RefPtr<UIScrollBar>			m_verticalScrollBar;
	IScrollInfo*				m_scrollTarget;
};


LN_NAMESPACE_END
