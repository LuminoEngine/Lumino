
#pragma once
#include "UIElement.h"
#include "UIButton.h"

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
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragStartedEvent);	/**< マウスドラッグを開始したときに発生するイベントを表します。*/
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragDeltaEvent);		/**< マウスドラッグ中に発生するイベントを表します。*/
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCompletedEvent);	/**< マウスドラッグが完了したときに発生するイベントを表します。*/
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCanceledEvent);	/**< マウスドラッグを中断したときに発生するイベントを表します。*/

public:
	static RefPtr<UIThumb> Create();

LN_CONSTRUCT_ACCESS:
	UIThumb();
	virtual ~UIThumb();
	void Initialize(detail::UIManager* manager);

protected:
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void OnDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragStartedEventId, this, e); } }
	virtual void OnDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragDeltaEventId, this, e); } }
	virtual void OnDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCompletedEventId, this, e); } }
	virtual void OnDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCanceledEventId, this, e); } }

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;

	UIEvent<UIDragDeltaEventArgs>		DragStarted;
	UIEvent<UIDragDeltaEventArgs>		DragDelta;
	UIEvent<UIDragDeltaEventArgs>		DragCompleted;
	UIEvent<UIDragDeltaEventArgs>		DragCanceled;
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

public:

	/** Track の方向を指定します。*/
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/** Track の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation GetOrientation() const { return m_orientation; }

	/** スクロール位置に対する値を設定します。*/
	void SetValue(float value) { m_value = value; }

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float GetValue() const { return m_value; }

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

LN_CONSTRUCT_ACCESS:
	UITrack();
	virtual ~UITrack();
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	void CoerceLength(float& componentLength, float trackLength);
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
	RefPtr<UIButton>	m_pageUpButton;
	RefPtr<UIThumb>		m_thumb;
	RefPtr<UIButton>	m_pageDownButton;
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
	@brief		ScrollBar のスクロールイベントの引数です。
*/
class UIScrollEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIScrollEventArgs(float newValue_, ScrollEventType type_) { newValue = newValue_; type = type_; }
	virtual ~UIScrollEventArgs() = default;

public:
	float			newValue;		/** 新しい値 */
	ScrollEventType	type;			/** スクロールイベントの原因 */
};

/**
	@brief		
*/
class UIScrollBar
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIScrollBar> Create();

	// TODO:↓後で RangeBase に移すかも

	/** スクロール位置に対する値を設定します。*/
	void SetValue(float value);

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float GetValue() const;

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
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void GetStyleClassName(String* outSubStateName);

private:
	void UpdateValue(float horizontalDragDelta, float verticalDragDelta);

	RefPtr<UITrack>	m_track;
	float			m_dragStartValue;
};

/**
	@brief		ScrollViewer コントロール内のメインのスクロール可能領域を表します。
*/
class IScrollInfo
{
public:

	/// 水平軸上にスクロールできるかどうかを示す値を設定します。
	/// @note	スクロールバーを表示しない設定になっていたり、そもそも存在しない場合に false がセットされる。
	///			IScrollInfo の実装クラスで不要な計算を行わないなどのために参照する。
	virtual void SetCanHorizontallyScroll(bool enabled) = 0;

	/// 水平軸上にスクロールできるかどうかを示す値を取得します。
	virtual bool CanHorizontallyScroll() const = 0;

	/// 垂直軸上にスクロールできるかどうかを示す値を設定します。
	virtual void SetCanVerticallyScroll(bool enabled) = 0;

	/// 垂直軸上にスクロールできるかどうかを示す値を取得します。
	virtual bool CanVerticallyScroll() const = 0;

	/// エクステント (コンテンツ全体) の横幅を取得します。
	virtual float GetExtentWidth() const = 0;

	/// エクステント (コンテンツ全体) の縦幅を取得します。
	virtual float GetExtentHeight() const = 0;

	/// コンテンツに対する実際の表示領域の横幅を取得します。
	virtual float GetViewportWidth() const = 0;

	/// コンテンツに対する実際の表示領域の縦幅を取得します。
	virtual float GetViewportHeight() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetHorizontalOffset(float offset) = 0;

	/// スクロールしたコンテンツの水平オフセットを取得します。
	virtual float GetHorizontalOffset() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetVerticalOffset(float offset) = 0;

	/// スクロールしたコンテンツの垂直オフセットを取得します。
	virtual float GetVerticalOffset() const = 0;

	/// スクロール動作を制御する ScrollViewer 要素を設定します。
	//virtual void SetScrollOwner(ScrollViewer* owner) = 0;

	/// コンテンツ内を 1 論理単位ずつ上にスクロールします。
	virtual void LineUp() = 0;

	/// コンテンツ内を 1 論理単位ずつ下にスクロールします。
	virtual void LineDown() = 0;

	/// コンテンツ内を 1 論理単位ずつ左にスクロールします。
	virtual void LineLeft() = 0;

	/// コンテンツ内を 1 論理単位ずつ右にスクロールします。
	virtual void LineRight() = 0;

#if 0
	/// コンテンツ内を 1 ページずつ上にスクロールします。
	virtual void PageUp() = 0;

	/// コンテンツ内を 1 ページずつ下にスクロールします。
	virtual void PageDown() = 0;

	/// コンテンツ内を 1 ページずつ左にスクロールします。
	virtual void PageLeft() = 0;

	/// コンテンツ内を 1 ページずつ右にスクロールします。
	virtual void PageRight() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、上にスクロールします。
	virtual void MouseWheelUp() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、下にスクロールします。
	virtual void MouseWheelDown() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、左にスクロールします。
	virtual void MouseWheelLeft() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、右にスクロールします。
	virtual void MouseWheelRight() = 0;
#endif

	//Rect MakeVisible(UIElement visual, Rect rectangle);
};

/**
	@brief		
*/
class UIScrollViewer
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIScrollViewer> Create();

LN_CONSTRUCT_ACCESS:
	UIScrollViewer();
	virtual ~UIScrollViewer();
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;

private:

	RefPtr<UIScrollBar>		m_verticalScrollBar;
	RefPtr<UIScrollBar>		m_horizontalScrollBar;
};

LN_NAMESPACE_END
