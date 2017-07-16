
#pragma once
#include "UIElement.h"
#include "UIButton.h"
#include "UIItemsControl.h"

LN_NAMESPACE_BEGIN
class IScrollInfo;
class UIScrollEventArgs;
using UIScrollEventArgsPtr = Ref<UIScrollEventArgs>;

/**
	@brief	マウスドラッグが関係するイベント引数です。
*/
class UIDragDeltaEventArgs
	: public UIEventArgs
{
	LN_OBJECT;
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
	LN_OBJECT;
public:
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragStartedEvent);		/**< マウスドラッグを開始したときに発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragDeltaEvent);			/**< マウスドラッグ中に発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCompletedEvent);		/**< マウスドラッグが完了したときに発生するイベントを表します。*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCanceledEvent);		/**< マウスドラッグを中断したときに発生するイベントを表します。*/

public:
	static Ref<UIThumb> create();

LN_CONSTRUCT_ACCESS:
	UIThumb();
	virtual ~UIThumb();
	void initialize();

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void onDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(DragStartedEventId, this, e); } }
	virtual void onDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(DragDeltaEventId, this, e); } }
	virtual void onDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(DragCompletedEventId, this, e); } }
	virtual void onDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { raiseEvent(DragCanceledEventId, this, e); } }

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
	LN_OBJECT;
public:

	// TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
	static const String OrientationStates;
	static const String HorizontalState;
	static const String VerticalState;


	static Ref<UITrack> create();
public:

	/** Track の方向を指定します。*/
	void setOrientation(Orientation orientation);

	/** Track の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation getOrientation() const { return m_orientation; }

	/** スクロール位置に対する値を設定します。*/
	void setValue(float value) { m_value = value; }

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float getValue() const { return m_value; }

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value) { m_minimum = value; }

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float getMinimum() const { return m_minimum; }

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value) { m_maximum = value; }

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float getMaximum() const { return m_maximum; }

	/** スクロール可能なコンテンツの中で表示される部分のサイズを設定します。*/
	void setViewportSize(float value) { m_viewportSize = value; }

	/** スクロール可能なコンテンツの中で表示される部分のサイズを取得します。*/
	float getViewportSize() const { return m_viewportSize; }

	/** 距離を値に変換します。*/
	float valueFromDistance(float horizontal, float vertical);

	UIThumb* getThumb() const;
	UIButton* getDecreaseButton() const;
	UIButton* getIncreaseButton() const;

LN_CONSTRUCT_ACCESS:

	UITrack();
	virtual ~UITrack();
	void initialize();

protected:
	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	void coerceLength(float& componentLength, float trackLength);
	void calcSliderComponentsSize(
		const Size& arrangeSize,
		bool isVertical,
		float* outDecreaseButtonLength,
		float* outThumbLength,
		float* outIncreaseButtonLength);
	void calcScrollBarComponentsSize(
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
	Ref<UIButton>	m_decreaseButton;
	Ref<UIThumb>		m_thumb;
	Ref<UIButton>	m_increaseButton;

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
	LN_OBJECT;
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
	LN_OBJECT;
public:
	LN_ROUTED_EVENT2(UIScrollEventArgs, ScrollEvent);

	// TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
	static const String OrientationStates;
	static const String HorizontalState;
	static const String VerticalState;




	static Ref<UIScrollBar> create();
	// TODO:↓後で RangeBase に移すかも


	/** ScrollBar の方向を指定します。*/
	void setOrientation(Orientation orientation);

	/** ScrollBar の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation getOrientation() const;

	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float getValue() const;

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value);

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float getMinimum() const;

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value);

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float getMaximum() const;

	/** スクロール可能なコンテンツの中で表示される部分のサイズを設定します。*/
	void setViewportSize(float value);

	/** スクロール可能なコンテンツの中で表示される部分のサイズを取得します。*/
	float getViewportSize() const;

LN_CONSTRUCT_ACCESS:
	UIScrollBar();
	virtual ~UIScrollBar();
	void initialize();

protected:
	// UIElement interface
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	//virtual void GetStyleClassName(String* outSubStateName);

private:
	void updateValue(float horizontalDragDelta, float verticalDragDelta);

	Ref<UITrack>		m_track;
	Ref<UIButton>	m_lineUpButton;
	Ref<UIButton>	m_lineDownButton;
	float				m_dragStartValue;
};

/**
	@brief
*/
class UIScrollViewer
	: public UIControl
{
	LN_OBJECT;
public:

	static Ref<UIScrollViewer> create();

LN_CONSTRUCT_ACCESS:
	UIScrollViewer();
	virtual ~UIScrollViewer();
	void initialize();

protected:
	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRoutedEvent(UIEventArgs* e) override;

	// UIControl interface
	virtual void onLayoutPanelChanged(UILayoutPanel* newPanel) override;

private:
	Ref<UIScrollBar>			m_horizontalScrollBar;
	Ref<UIScrollBar>			m_verticalScrollBar;
	IScrollInfo*				m_scrollTarget;
};


LN_NAMESPACE_END
