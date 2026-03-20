#pragma once
#include "../UIEvents.hpp"
#include "UIControl.hpp"

namespace ln {
class UIButton;
class IScrollInfo;

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
	virtual const String& elementName() const  override { static String name = _TT("UIThumb"); return name; }
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

/**　特定の範囲内の値を持つ要素を表します。 */
class UIRangeBase
	: public UIElement
{
public:
	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

	/** スクロール位置に対する値を取得します。(default: 0.0) */
	float value() const;

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value);

	/** 指定可能な最小値を取得します。(default: 0.0) */
	float minimum() const;

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value);

	/** 指定可能な最大値を取得します。(default: 1.0) */
	float maximum() const;

protected:
	/** 値が変更されたときに呼び出されます。*/
	virtual void onValueChanged(float oldValue, float newValue);

	/** 指定可能な最小値が変更されたときに呼び出されます。*/
	virtual void onMinimumChanged(float oldMinimum, float newMinimum);

	/** 指定可能な最大値が変更されたときに呼び出されます。*/
	virtual void onMaximumChanged(float oldMaximum, float newMaximum);

LN_CONSTRUCT_ACCESS:
	UIRangeBase();
	virtual ~UIRangeBase();
	void init();

private:
	float m_value;
	float m_minimum;
	float m_maximum;
};

/**
	@brief
*/
class UITrack
	: public UIElement	// TODO: UIElement にしたい気持ち。今は init で addElement している個所を、addVisualChild とかにしたい。
								// ※VisualChild はその要素を表現するために深く紐づいた子要素。Window のクライアント領域外のボタンとかが相当する。
{
	//LN_OBJECT;
public:

	// TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
	static const String OrientationStates;
	static const String HorizontalState;
	static const String VerticalState;


	//static Ref<UITrack> create();
public:

	/** Track の方向を指定します。*/
	void setOrientation(UILayoutOrientation orientation);

	/** Track の方向を取得します。規定値は UILayoutOrientation::Horizontal です。*/
	UILayoutOrientation getOrientation() const { return m_orientation; }

	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

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
	void init();

protected:
	virtual const String& elementName() const  override { static String name = _TT("UITrack"); return name; }

	// UIElement interface
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

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

	UILayoutOrientation			m_orientation;
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	float				m_density;
	float				m_viewportSize;
	Ref<UIButton>	m_decreaseButton;
	Ref<UIThumb>		m_thumb;
	Ref<UIButton>	m_increaseButton;

};


/**
    @brief
*/
// 方針としては、「SliderMode でも ScrollBarMode でも、Thumb が下端に付いたら Maximum の値になる」
// なので、Scrollbar を作るときに ExtentSize をそのまま Maximum にしたりすると、
// オフセットの最大値が ExtentSize になるので、Thumb を下端に持ってくると Extent 全体がViewport外にスライドする。これは仕様。
// Maximum には ExtentSize - ViewportSize を設定する必要がある。
class UIScrollBar
    : public UIElement
{
public:

    // TODO: 別途、UIScrollBarStyle 見たいなクラスに分けたほうがいいかも
    static const String OrientationStates;
    static const String HorizontalState;
    static const String VerticalState;




    // TODO:↓後で RangeBase に移すかも


    /** ScrollBar の方向を指定します。*/
    void setOrientation(UILayoutOrientation orientation);

    /** ScrollBar の方向を取得します。規定値は UILayoutOrientation::Horizontal です。*/
    UILayoutOrientation getOrientation() const;

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
    void init();

protected:
    // UIElement interface
	virtual const String& elementName() const  override { static String name = _TT("UIScrollBar"); return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

private:
    void updateValue(float horizontalDragDelta, float verticalDragDelta);

    Ref<UITrack> m_track;
    Ref<UIButton> m_lineUpButton;
    Ref<UIButton> m_lineDownButton;
    float m_dragStartValue;
};


// TODO: detail
// WPF のように UIScrollViewer を UI ツリーに含めると更新のオーバーヘッドが増える。
// ただスクロールバー出すためだけに複雑なことしたくないので、ListView とか TreeView に簡単に
// スクロールバーを付けるためのユーティリティ。
class UIScrollViewHelper
	: public RefObject
{
public:
	UIScrollViewHelper(UIControl* owner);
	void setHScrollbarVisible(bool value);
	void setVScrollbarVisible(bool value);
	void setScrollTarget(IScrollInfo* value) { m_scrollTarget = value; }
	Size measure(UILayoutContext* layoutContext, const Size& ownerAreaSize);
	Rect calculateClientArea(const Size& ownerAreaSize);
	void arrange(UILayoutContext* layoutContext, const Size& ownerAreaSize);
	void refreshScrollInfo();
	void handleRoutedEvent(UIEventArgs* e);

private:
	UIControl* m_owner;
	Ref<UIScrollBar> m_horizontalScrollBar;
	Ref<UIScrollBar> m_verticalScrollBar;
	IScrollInfo* m_scrollTarget;
};


/**
    @brief
*/
// TODO: obsolete: UIScrollViewHelper を内包するように修正する
class UIScrollViewer
    : public UIControl
{
public:

    //static Ref<UIScrollViewer> create();

	void setHScrollbarVisible(bool value);
	void setVScrollbarVisible(bool value);

LN_CONSTRUCT_ACCESS:
    UIScrollViewer();
    virtual ~UIScrollViewer();
    void init();

protected:
    // UIElement interface
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    Ref<UIScrollBar>			m_horizontalScrollBar;
    Ref<UIScrollBar>			m_verticalScrollBar;
    IScrollInfo*				m_scrollTarget;
};


} // namespace ln

