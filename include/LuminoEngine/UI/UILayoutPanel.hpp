#pragma once
#include "UIElement.hpp"
#include "UILayoutElement.hpp"

namespace ln {


// TODO: GridLayout https://blog.qt.io/jp/2011/01/06/qml-layout/

class IUIElementList
{
public:
    virtual int getElementCount() const = 0;
    virtual UIElement* getElement(int i) const = 0;
};


/**
    @brief        ScrollViewer コントロール内のメインのスクロール可能領域を表します。
*/
// offset はスクロールバー基準。縦スクロールバーなら、Thumb が下に行くほど、VerticalOffset には + 方向の大きい値がセットされる。
class IScrollInfo
{
public:

    ///// 水平軸上にスクロールできるかどうかを示す値を設定します。
    ///// @note    スクロールバーを表示しない設定になっていたり、そもそも存在しない場合に false がセットされる。
    /////            IScrollInfo の実装クラスで不要な計算を行わないなどのために参照する。
    //virtual void SetCanHorizontallyScroll(bool enabled) = 0;

    ///// 水平軸上にスクロールできるかどうかを示す値を取得します。
    //virtual bool CanHorizontallyScroll() const = 0;

    ///// 垂直軸上にスクロールできるかどうかを示す値を設定します。
    //virtual void SetCanVerticallyScroll(bool enabled) = 0;

    ///// 垂直軸上にスクロールできるかどうかを示す値を取得します。
    //virtual bool CanVerticallyScroll() const = 0;

    /// エクステント (コンテンツ全体) の横幅を取得します。
    virtual float getExtentWidth() const = 0;

    /// エクステント (コンテンツ全体) の縦幅を取得します。
    virtual float getExtentHeight() const = 0;

    /// コンテンツに対する実際の表示領域の横幅を取得します。
    virtual float getViewportWidth() const = 0;

    /// コンテンツに対する実際の表示領域の縦幅を取得します。
    virtual float getViewportHeight() const = 0;

    /// スクロールしたコンテンツの水平オフセットを設定します。
    virtual void setHorizontalOffset(float offset) = 0;

    /// スクロールしたコンテンツの水平オフセットを取得します。
    virtual float getHorizontalOffset() const = 0;

    /// スクロールしたコンテンツの水平オフセットを設定します。
    virtual void setVerticalOffset(float offset) = 0;

    /// スクロールしたコンテンツの垂直オフセットを取得します。
    virtual float getVerticalOffset() const = 0;

    /// スクロール動作を制御する ScrollViewer 要素を設定します。
    //virtual void SetScrollOwner(ScrollViewer* owner) = 0;

#if 0
/// コンテンツ内を 1 論理単位ずつ上にスクロールします。
    virtual void LineUp() = 0;

    /// コンテンツ内を 1 論理単位ずつ下にスクロールします。
    virtual void LineDown() = 0;

    /// コンテンツ内を 1 論理単位ずつ左にスクロールします。
    virtual void LineLeft() = 0;

    /// コンテンツ内を 1 論理単位ずつ右にスクロールします。
    virtual void LineRight() = 0;

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





// TODO: ネスト
class UILayoutPanel2
    : public Object
    , public IScrollInfo
{
public:
    UILayoutPanel2();
    void init();

    /** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
    const Size& desiredSize() const { return m_desiredSize; }


    // TODO: internal
    void measureLayout(const IUIElementList* childElements, const Size& availableSize);
    void arrangeLayout(const IUIElementList* childElements, const Rect& finalSlotRect);

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) = 0;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) = 0;

protected:
    // IScrollInfo interface
    virtual float getExtentWidth() const;
    virtual float getExtentHeight() const;
    virtual float getViewportWidth() const;
    virtual float getViewportHeight() const;
    virtual void setHorizontalOffset(float offset);
    virtual float getHorizontalOffset() const;
    virtual void setVerticalOffset(float offset);
    virtual float getVerticalOffset() const;

public:	// TODO: internal

private:
    Size m_desiredSize;
    Size m_actualSize;
    Vector2 m_scrollOffset;
};

class UIFrameLayout2	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel2
{
public:
    static Ref<UIFrameLayout2> create();

LN_CONSTRUCT_ACCESS:
    UIFrameLayout2();
    virtual ~UIFrameLayout2();
    void init();

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
};

class UIStackLayout2
    : public UILayoutPanel2
{
public:
    static Ref<UIStackLayout2> create();

    void setOrientation(Orientation orientation) { m_orientation = orientation; }
    Orientation getOrientation() const { return m_orientation; }

LN_CONSTRUCT_ACCESS:
    UIStackLayout2();
    virtual ~UIStackLayout2();
    void init();

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
    Orientation m_orientation;
};

} // namespace ln

