#pragma once
#include "UIElement.hpp"
#include "UILayoutElement.hpp"

namespace ln {


// TODO: GridLayout https://blog.qt.io/jp/2011/01/06/qml-layout/








// TODO: ネスト
class UILayoutPanel2
    : public Object
{
public:
    UILayoutPanel2();
    void init();

    /** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
    const Size& desiredSize() const { return m_desiredSize; }


    // TODO: internal
    void measureLayout(const List<Ref<UIElement>>& childElements, const Size& availableSize);
    void arrangeLayout(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect);

    virtual Size measureOverride(const List<Ref<UIElement>>& childElements, const Size& constraint) = 0;
    virtual void arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect) = 0;

protected:

public:	// TODO: internal

private:
    Size m_desiredSize;
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

    virtual Size measureOverride(const List<Ref<UIElement>>& childElements, const Size& constraint) override;
    virtual void arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect) override;

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

    virtual Size measureOverride(const List<Ref<UIElement>>& childElements, const Size& constraint) override;
    virtual void arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect) override;

private:
    Orientation m_orientation;
};

} // namespace ln

