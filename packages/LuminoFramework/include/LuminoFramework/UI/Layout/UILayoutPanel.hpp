#pragma once
#include <LuminoEngine/Base/Collection.hpp>
#include "../UIElement.hpp"
#include "../UILayoutElement.hpp"

namespace ln {
class UIItemsControl;

// TODO: GridLayout https://blog.qt.io/jp/2011/01/06/qml-layout/



/** グリッドレイアウトのセルサイズを指定する値の種類です。*/
enum class UILayoutLengthType
{
    Auto,				/**< 子要素のサイズに合わせる */
    Direct,				/**< サイズを直接指定する */
    Ratio,				/**< レイアウト後、残りの領域を使う */
};

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





/**
 * UILayoutPanel
 */
LN_CLASS()
class UILayoutPanel
    : public UIElement
    , public IScrollInfo
{
    LN_OBJECT;
public:
	//class Builder : public UIElement::Builder
	//{
	//	LN_BUILDER(UILayoutPanel);
	//public:
	//	Builder& children(std::initializer_list<UIElement::Builder> list);

	//protected:
	//	class Details : public UIElement::Builder::Details
	//	{
	//	public:
	//		List<UIElement::Builder> children;
	//		virtual Ref<Object> build() override;
	//	};
	//};

    UILayoutPanel();
    bool init();

    void removeChild(UIElement* child);
    void removeAllChildren();

protected:
    void onDispose(bool explicitDisposing) override;
    void onAddChild(UIElement* child) override;

    // IScrollInfo interface
    float getExtentWidth() const override;
    float getExtentHeight() const override;
    float getViewportWidth() const override;
    float getViewportHeight() const override;
    void setHorizontalOffset(float offset) override;
    float getHorizontalOffset() const override;
    void setVerticalOffset(float offset) override;
    float getVerticalOffset() const override;

public:	// TODO:
    Ref<Collection<Ref<UIElement>>> m_logicalChildren;
    //List<Ref<UIElement>> m_logicalChildren;
    Vector2 m_scrollOffset;
    UIItemsControl* m_ownerItemsControl;

    friend class UIItemsControl;
};

class UIFrameLayout2	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel
{
public:
    static Ref<UIFrameLayout2> create();

    // TODO: internal
    template<class TElement>
    static Size staticMeasureLogicalChildrenAreaSize(UILayoutContext* layoutContext, TElement* ownerElement, const Size& constraint)
    {
        int childrenCount = (ownerElement->m_logicalChildren) ? ownerElement->m_logicalChildren->size() : 0;//->getVisualChildrenCount();
        Size childMaxSize(0, 0);
        for (int i = 0; i < childrenCount; i++)
        {
            UIElement* child = ownerElement->m_logicalChildren->at(i);//ownerElement->getVisualChild(i);
            if (layoutContext->testLayoutEnabled(child)) {
                child->measureLayout(layoutContext, constraint);
                const Size& desiredSize = child->desiredSize();
                childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
                childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
            }
        }
        return childMaxSize;
    }


    template<class TElement>
    static Size staticMeasureLogicalChildren(UILayoutContext* layoutContext, TElement* ownerElement, const Size& constraint)
    {
        Size childMaxSize = staticMeasureLogicalChildrenAreaSize(layoutContext, ownerElement, constraint);
        return layoutContext->makeDesiredSize(ownerElement, childMaxSize);
    }
    
    // finalArea: padding 適用済み
    template<class TElement>
    static Size staticArrangeLogicalChildrenArea(UILayoutContext* layoutContext, TElement* ownerElement, const Rect& finalArea)
    {
        int childrenCount = (ownerElement->m_logicalChildren) ? ownerElement->m_logicalChildren->size() : 0;////getVisualChildrenCount();
        for (int i = 0; i < childrenCount; i++)
        {
            UIElement* child = ownerElement->m_logicalChildren->at(i);//getVisualChild(i);
            if (layoutContext->testLayoutEnabled(child)) {

                Rect slotRect;
                detail::LayoutHelper::adjustAlignment(
                    finalArea, child->desiredSize(),
                    ownerElement->m_finalStyle->horizontalContentAlignment, ownerElement->m_finalStyle->verticalContentAlignment, &slotRect);

                child->arrangeLayout(layoutContext, slotRect);
            }
        }
        return finalArea.getSize();
    }
    
    template<class TElement>
    static Size staticArrangeLogicalChildren(UILayoutContext* layoutContext, TElement* ownerElement, const Rect& finalArea)
    {
        // padding 等を適用する
        Rect contentArea = detail::LayoutHelper::arrangeClientArea(ownerElement, finalArea);

        staticArrangeLogicalChildrenArea(layoutContext, ownerElement, contentArea);

        // ローカルオフセットも含めて、消費した領域全体を返す
        //return Size(contentArea.x + contentArea.width, contentArea.y + contentArea.height);
        return finalArea.getSize();
    }

LN_PROTECTED_INTERNAL_ACCESS:
    UIFrameLayout2();
    void init();

    // UIElement interface
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

private:
};

// TODO: 一方向の GridLayout は BoxLayout (UE4) にする。これは、指定方向のレイアウト領域を inf にしない。
// 一方、WPF の StackPanel のように、細かい配置の調整はせずただ並べるだけのものは LinearLayout(Andorid) にしてみる。これは指定方向のレイアウト領域が inf になる。
// おもに ListView など用。
class UIStackLayout2_Obsolete
    : public UILayoutPanel
{
public:
    static Ref<UIStackLayout2_Obsolete> create();

    void setOrientation(UILayoutOrientation orientation) { m_orientation = orientation; }
    UILayoutOrientation getOrientation() const { return m_orientation; }

    void addChild(UIElement* element) { addChild(element, UILayoutLengthType::Auto); }
    void addChild(UIElement* element, UILayoutLengthType type);

LN_CONSTRUCT_ACCESS:
    UIStackLayout2_Obsolete();
    void init();

    // UIElement interface
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

private:
    bool isHorizontal() const { return m_orientation == UILayoutOrientation::Horizontal || m_orientation == UILayoutOrientation::ReverseHorizontal; }

    struct CellDefinition
    {
        // input data (initial data)
        UILayoutLengthType type = UILayoutLengthType::Auto;
        float size = 1.0f;
        float minSize = 0.0f;
        float maxSize = std::numeric_limits<float>::max();

        // working data
        float desiredSize = Math::NaN;
        float actualOffset = Math::NaN;
        float actualSize = Math::NaN;
    };

    UILayoutOrientation m_orientation;
    List<CellDefinition> m_cellDefinitions;
};

class UIHBoxLayout2
    : public UIStackLayout2_Obsolete
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};

class UIVBoxLayout2
    : public UIStackLayout2_Obsolete
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};


/**
 * UIBoxLayout
 *
 * 指定方向に無限のサイズを想定する StackLayout と異なり、こちらは有限サイズでレイアウトを行う。
 * 特に末尾の要素を下揃えしたり、指定方向への Stratch をできるようにしたりする。
 * このため ListBox などの HostPanel には向かない。
 * 
 * 例：2分割で、２つ目のセルを Stretch したい場合は、そこに配置する要素の Alignment を Stretch にする。
 */
LN_CLASS()
class UIBoxLayout
    : public UILayoutPanel
{
    LN_OBJECT;
public:
    /** レイアウト方向を設定します。(default: Vertical) */
    LN_METHOD(Property)
    void setOrientation(UILayoutOrientation orientation) { m_orientation = orientation; }

    /** レイアウト方向を取得します。 */
    LN_METHOD(Property)
    UILayoutOrientation orientation() const { return m_orientation; }

LN_CONSTRUCT_ACCESS:
    UIBoxLayout();

    /** init */
    LN_METHOD()
    void init();

    // UIElement interface
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

private:
    bool isHorizontal() const { return m_orientation == UILayoutOrientation::Horizontal || m_orientation == UILayoutOrientation::ReverseHorizontal; }
    UILayoutLengthType layoutType(int index) const;
    float layoutWeight(int index) const;
    float layoutDirectSize(int index) const;
    void getLayoutMinMaxSize(int index, float* minSize, float* maxSize) const;

    struct CellDefinition
    {
        float desiredSize = Math::NaN;
        float actualOffset = Math::NaN;
        float actualSize = Math::NaN;
    };

    UILayoutOrientation m_orientation;
    List<CellDefinition> m_cellDefinitions;
};

class UIHBoxLayout3
    : public UIBoxLayout
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};


class UIVBoxLayout3
    : public UIBoxLayout
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};








class UISwitchLayout
	: public UIFrameLayout2
{
public:
	void setActiveIndex(int index);
    void setActive(UIElement* child);

protected:
	// base interface
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;

LN_CONSTRUCT_ACCESS:
	UISwitchLayout();
    void init();

private:
	int m_activeIndex;
};





// TODO: ネスト
class UILayoutPanel2_Deprecated
    : public Object
    , public IScrollInfo
{
public:
    UILayoutPanel2_Deprecated();
    void init();

    /** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
    const Size& desiredSize() const { return m_desiredSize; }


    // TODO: internal
    void measureLayout(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& availableSize);
    void arrangeLayout(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect);

    virtual Size measureOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& constraint) = 0;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect) = 0;

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

class UIFrameLayout	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel2_Deprecated
{
public:
    static Ref<UIFrameLayout> create();

LN_CONSTRUCT_ACCESS:
    UIFrameLayout();
    virtual ~UIFrameLayout();
    void init();

    virtual Size measureOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
};

// UILayoutOrientation=H で大量アイテムを持つ ListView を配置すると、縦方向スクロールバーが表示される。
// UILayoutOrientation=V で大量アイテムを持つ ListView を配置すると、スクロールバーは表示されず、すべてのアイテムを包含するサイズが ListView の actualSize となる。（見切れる）
class UIStackLayout_Obsolete
    : public UILayoutPanel2_Deprecated
{
public:
    static Ref<UIStackLayout_Obsolete> create();

    void setOrientation(UILayoutOrientation orientation) { m_orientation = orientation; }
    UILayoutOrientation getOrientation() const { return m_orientation; }

    bool lastStretch = false;

LN_CONSTRUCT_ACCESS:
    UIStackLayout_Obsolete();
    virtual ~UIStackLayout_Obsolete();
    void init();

    virtual Size measureOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
    UILayoutOrientation m_orientation;
};

//class UIVBoxLayout
//    : public UIStackLayout_Obsolete
//{
//public:
//
//LN_CONSTRUCT_ACCESS:
//    void init();
//};

/**
 * UIStackLayout
 */
LN_CLASS()
class UIStackLayout
    : public UILayoutPanel
{
public:
    static Ref<UIStackLayout> create();
    static Ref<UIStackLayout> create(UILayoutOrientation orientation);

    /**  (default: Vertical) */
    LN_METHOD(Property)
    void setOrientation(UILayoutOrientation orientation) { m_orientation = orientation; }

    LN_METHOD(Property)
    UILayoutOrientation getOrientation() const { return m_orientation; }

    bool lastStretch = false;

    // TODO: intenal
    static Size measureOverrideImpl(UILayoutContext* layoutContext, const Size& constraint, int itemCount, const std::function<UIElement*(int i)>& getItem, UILayoutOrientation orientation);
    static Size arrangeOverrideImpl(
        UILayoutContext* layoutContext, const Rect& finalArea,
        int itemCount, const std::function<UIElement*(int i)>& getItem,
        UILayoutOrientation orientation, detail::UIStyleInstance* style,
        bool lastStretch, const Vector2& scrollOffset);

LN_CONSTRUCT_ACCESS:
    UIStackLayout();

    /** init */
    LN_METHOD()
    bool init();

    bool init(UILayoutOrientation orientation);

    Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

private:
    UILayoutOrientation m_orientation;
};


} // namespace ln

