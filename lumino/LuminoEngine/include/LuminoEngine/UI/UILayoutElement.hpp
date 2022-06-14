#pragma once
#include <LuminoGraphics/Rendering/Common.hpp>
#include "Common.hpp"

namespace ln {
class Font;
class Texture;
class Shader;
class Material;
class UIStyle;
class UILayoutContext;
class UIElement;
class UITreeView2;
namespace detail { class LayoutHelper; }

enum class UIStyleValueUnit {
    Point,
    Percent,
};

class UIStyleValue {
public:
    static const UIStyleValue Null;

    constexpr UIStyleValue(float pointValue) noexcept
        : m_value(pointValue)
        , m_unit(UIStyleValueUnit::Point) {}

    constexpr UIStyleValue() noexcept
        : m_value(std::numeric_limits<float>::quiet_NaN())
        , m_unit(UIStyleValueUnit::Point) {}

    constexpr UIStyleValue(float value, UIStyleValueUnit unit) noexcept
        : m_value(value)
        , m_unit(unit) {}

    constexpr float value() const noexcept { return m_value; }

    float valueOr(float defaultValue) const noexcept { return isNull() ? defaultValue : m_value; }

    constexpr UIStyleValueUnit unit() const noexcept { return m_unit; }

    bool isNull() const noexcept { return std::isnan(m_value); }

    bool isPercent() const { return m_unit == UIStyleValueUnit::Percent; }

    static constexpr UIStyleValue ofNull() noexcept { return UIStyleValue{}; }

    static constexpr UIStyleValue makePercent(float percent) noexcept { return UIStyleValue(percent, UIStyleValueUnit::Percent); }
    //static inline constexpr UIStyleValue Null = UIStyleValue();

private:
    float m_value;
    UIStyleValueUnit m_unit;
};




enum class UIFontWeight
{
	Normal,
	Bold,
};

enum class UIFontStyle
{
	Normal,
	Italic,
};

/** 縦方向の表示位置を示します。*/
LN_ENUM()
enum class UIVAlignment
{
	/** 子要素を、親のレイアウト スロットの上端に揃えて配置します。*/
	Top = 0,

	/** 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Center,

	/** 子要素を、親のレイアウト スロットの下端に揃えて配置します。*/
	Bottom,

	/** 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
	Stretch,
};

/** 横方向の表示位置を示します。*/
LN_ENUM()
enum class UIHAlignment
{
	/** 子要素を、親のレイアウト スロットの左側に揃えて配置します。*/
	Left = 0,

	/** 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Center,

	/** 子要素を、親のレイアウト スロットの右側に揃えて配置します。*/
	Right,

	/** 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
	Stretch,
};

/** UI要素の表示位置を示します。*/
LN_ENUM()
enum class UIAlignment {
    /** 子要素を、親のレイアウト スロットの左側に揃えて配置します。*/
    Left = 0,

    /** 子要素を、親のレイアウト スロットの上端に揃えて配置します。*/
    Top,

    /** 子要素を、親のレイアウト スロットの右側に揃えて配置します。*/
    Right,

    /** 子要素を、親のレイアウト スロットの下端に揃えて配置します。*/
    Bottom,

    /** 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
    Center,

    /** 子要素を、親のレイアウト スロットの左上に揃えて配置します。*/
    TopLeft,

    /** 子要素を、親のレイアウト スロットの右上に揃えて配置します。*/
    TopRight,

    /** 子要素を、親のレイアウト スロットの左下に揃えて配置します。*/
    BottomLeft,

    /** 子要素を、親のレイアウト スロットの右下に揃えて配置します。*/
    BottomRight,

	HorizontalStretch,

    VerticalStretch,

    /** 子要素を、親のレイアウト スロットの左側に揃え、上下を引き延ばして配置します。*/
    LeftStretch,

    /** 子要素を、親のレイアウト スロットの上側に揃え、左右を引き延ばして配置します。*/
    TopStretch,

    /** 子要素を、親のレイアウト スロットの右側に揃え、上下を引き延ばして配置します。*/
    RightStretch,

    /** 子要素を、親のレイアウト スロットの下側に揃え、左右を引き延ばして配置します。*/
    BottomStretch,

    /** 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
    Stretch,
};


namespace detail
{
class UIStyleInstance;
//struct UIInheritStyleAttribute
//{
//	Color textColor;
//	String fontFamily;
//	float fontSize;
//	UIFontStyle fontStyle;
//};

//struct StyleData
//{
//	UIStyle* sourceLocalStyle;	// 以下のデータの生成元となったローカスのスタイル
//
//	// layout
//	Thickness margin;
//	Thickness padding;
//	UIHAlignment hAlignment;
//	UIVAlignment vAlignment;
//	float minWidth;
//	float minHeight;
//	float maxWidth;
//	float maxHeight;
//
//	// layout transform
//	Vector3 position;
//	Quaternion rotation;
//	Vector3 scale;
//	Vector3 centerPoint;
//
//	// background
//	Color backgroundColor;
//	//Ref<Texture> backgroundImage;
//	//Ref<Shader> backgroundShader;
//	Ref<Material> backgroundMaterial;
//
//	// text
//	Color textColor;
//	Ref<Font> font;
//	//String fontFamily;
//	//float fontSize;
//	//UIFontWeight fontWeight;
//	//UIFontStyle fontStyle;
//
//	// render effects
//	bool visible;
//	BlendMode blendMode;
//
//	float opacity;
//	Color colorScale;
//	Color blendColor;
//	ColorTone tone;
//
//	// TODO: 今後サブクラスごとにスタイルを追加する場合は、ここに map を設ける
//};
}

LN_CLASS()
class UILayoutElement
	: public Object
{
    LN_OBJECT;
public:	// TODO: internal
	UILayoutElement();
	void init() { LN_UNREACHABLE(); }	// dummy
	void init(const detail::UIStyleInstance* finalStyle);

	// 基本的にルート要素のみ呼び出すべき
	void updateLayout(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);



	/**
	 * レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。
	 *
	 * この値は子要素が親要素へ要求する、子要素自身の最低サイズです。
	 * 必ずしも width,height プロパティや actual-size と一致するものではありません。
	 * margin, padding, border を考慮した、要素を配置する領域の要求サイズです。
	 */
	const Size& desiredSize() const { return m_desiredSize; }

	// arrangeOverride の戻り値。
	// border + padding + ContentSize. margin は含まれない。
	// つまり、この要素を描画するために必要な領域を示す。
    const Size& actualSize() const { return m_actualSize; }

	/**
	 * 要素のクライアント領域を返します。座標は要素の境界領域内の相対座標です。
	 *
	 * クライアント領域は、境界領域に Border 幅を適用したものです。
	 */
	Rect clientRect_Obsolete() const;

	/**
	 * 要素のコンテンツ領域を返します。座標は要素の境界領域内の相対座標です。
	 *
	 * コンテンツ領域は、境界領域に Border 幅と Padding を適用したものです。
	 */
	Rect contentRect_Obsolete() const;

	//const Rect& finalGlobalRect() const { return m_finalGlobalRect; }

    // TODO: internal
    const Point& localPosition() const { return m_localPosition; }

protected:
	virtual ~UILayoutElement();

	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint);
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea);

	//virtual int getChildLayoutItemCount() const = 0;
	//virtual ILayoutElement* getChildLayoutItem(int index) const = 0;


	virtual void onUpdateLayout(UILayoutContext* layoutContext);


public:	//TODO: internal
	void measureLayout(UILayoutContext* layoutContext, const Size& availableSize);
	virtual void arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect);
	///*virtual*/ void updateFinalRects(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);
    void updateFinalRects(UILayoutContext* layoutContext, const Matrix& parentCombinedRenderTransform);

	//const Size& getLayoutSize() const { return m_layoutSize; }
	void setLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
	const Size& getLayoutDesiredSize() const { return m_desiredSize; }
	const Thickness& getLayoutMargin() const;
	const Thickness& getLayoutPadding() const;
	UIHAlignment getLayoutHAlignment() const;
	UIVAlignment getLayoutVAlignment() const;
	void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const;

	//void setLayoutFinalLocalRect(const Rect& rect) { m_finalLocalRect = rect; }
	//const Rect& getLayoutFinalLocalRect() const { return m_finalLocalRect; }
	//void setLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }

	//Size m_layoutSize;
	const detail::UIStyleInstance* m_finalStyle;
	Size m_desiredSize; // includes, margin, border

    
	// メインのレイアウトツリー上での座標変換に使用する。特にマウス位置。
    Matrix m_combinedFinalRenderTransform;

	Matrix m_localTransform;

private:
    Point m_localPosition;  // 親コンテナ内の相対座標
    Size m_actualSize;
    //Rect m_finalGlobalRect; // TODO: obsolete



	friend class detail::LayoutHelper;
};



class UILayoutContext
	: public Object
{
public:
	float dpiScale() const { return m_dpiScale; }
	UIStyleContext* styleContext() const { return m_styleContext; }

	bool testLayoutEnabled(UIElement* element) const;

	// TreeItem の追加などで、TreeView の VisualChild としての TreeItem を再集計したいかどうか。
	// 再収集は measure の中でやりたいので、UILayoutContext にフラグを持たせている。
	// ※別に measure じゃなくてもツリーを走査して再構築する別なフローを作ってもいいのだが、パフォーマンスへの影響が心配。
	//bool collectingTreeItem = false;
	//UITreeView2* treeView = nullptr;

	// measureOverride() や arrangeOverride() でコンテンツ領域を計算するユーティリティ
	// (コンテンツ領域 = 論理的な子要素を配置する領域。Chrome DevTool で示される青色の部分)
	//Rect makeContentAreaRect(const UIElement* element, const Size& borderRect) const;

	// measureOverride() の戻り値を計算するユーティリティ
	Size makeDesiredSize(const UIElement* element, const Size& contentAreaSize) const;

	// arrangeOverride() のユーティリティ
	Rect makeContentRect(const UIElement* element, const Size& finalSize) const;

	Size measureTextSize(Font* font, const StringView& text);
	Size measureTextSize(Font* font, uint32_t codePoint);
	Size measureTextSize(const UIElement* element, const StringView& text);
	Size measureTextSize(const UIElement* element, uint32_t codePoint);

LN_CONSTRUCT_ACCESS:
	UILayoutContext();
	void init();

private:
	float m_dpiScale;
	UIStyleContext* m_styleContext = nullptr;
	friend class UIFrameWindow;
};

class ILayoutPanel
{
protected:
    ILayoutPanel();

private:
};

namespace detail {

class LayoutHelper
{
public:

    static Size measureElementSpacing(UILayoutElement* element);    // border + padding
    static Size measureElementClientSize(UILayoutElement* element);    // chrome デバッガの青い部分
	// 単純に element のサイズによった measure を行う。measureOverride() の中で使用することを想定している。
	static Size measureElement(UILayoutElement* element, const Size& constraint, const Size& childrenDesiredSize);

    // padding, border の分を減じた領域を計算する
	// Note: TreeItem や Window で、クライアント領域のオフセットをつけるため、final は size ではなく area
	static Rect arrangeClientArea(UILayoutElement* element, const Rect& finalArea);

	//static void forEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	//{
	//	int count = element->getVisualChildrenCount();
	//	for (int i = 0; i < count; ++i)
	//	{
	//		func(element->getVisualChild(i));
	//	}
	//}

	// widthNan : ユーザーが希望するサイズを指定しているか
	static void adjustHorizontalAlignment(const Size& areaSize, const Size& desiredSize, float fixedSizeOrNaN, UIHAlignment align, Rect* outRect)
	{
		switch (align)
		{
		case UIHAlignment::Left:
			outRect->x = 0;
			outRect->width = desiredSize.width;
			break;
		case UIHAlignment::Center:
			outRect->x = (areaSize.width - desiredSize.width) / 2;
			outRect->width = desiredSize.width;
			break;
		case UIHAlignment::Right:
			outRect->x = areaSize.width - desiredSize.width;
			outRect->width = desiredSize.width;
			break;
		case UIHAlignment::Stretch:
			if (Math::isNaN(fixedSizeOrNaN))
			{
				outRect->x = 0;
				outRect->width = areaSize.width;
			}
			else
			{
				outRect->x = (areaSize.width - desiredSize.width) / 2;
				outRect->width = fixedSizeOrNaN;
			}

			//outRect->x = 0;
			//outRect->width = areaSize.width;
			break;
		}
	}

	static void adjustVerticalAlignment(const Size& areaSize, const Size& desiredSize, float fixedSizeOrNaN, UIVAlignment align, Rect* outRect)
	{
		switch (align)
		{
		case UIVAlignment::Top:
			outRect->y = 0;
			outRect->height = desiredSize.height;
			break;
		case UIVAlignment::Center:
			outRect->y = (areaSize.height - desiredSize.height) / 2;
			outRect->height = desiredSize.height;
			break;
		case UIVAlignment::Bottom:
			outRect->y = areaSize.height - desiredSize.height;
			outRect->height = desiredSize.height;
			break;
		case UIVAlignment::Stretch:
            if (Math::isNaN(fixedSizeOrNaN))
			{
				outRect->y = 0;
				outRect->height = areaSize.height;
			}
			else
			{
				outRect->y = (areaSize.height - desiredSize.height) / 2;
				outRect->height = fixedSizeOrNaN;
			}

			break;
		}
	}

    static void adjustAlignment(const Rect& area, const Size& desiredSize, UIHAlignment halign, UIVAlignment valign, Rect* outRect)
    {
        assert(!Math::isNaNOrInf(area.width));
        assert(!Math::isNaNOrInf(area.height));
        assert(!Math::isNaNOrInf(desiredSize.width));
        assert(!Math::isNaNOrInf(desiredSize.height));
        assert(outRect);

        switch (halign)
        {
        case UIHAlignment::Left:
            outRect->x = area.x;
            outRect->width = desiredSize.width;
            break;
        case UIHAlignment::Center:
            outRect->x = area.x + ((area.width - desiredSize.width) / 2);
            outRect->width = desiredSize.width;
            break;
        case UIHAlignment::Right:
            outRect->x = area.x + (area.width - desiredSize.width);
            outRect->width = desiredSize.width;
            break;
        case UIHAlignment::Stretch:
            outRect->x = area.x;
            outRect->width = area.width;
            break;
        default:
            LN_UNREACHABLE();
            break;
        }

        switch (valign)
        {
        case UIVAlignment::Top:
            outRect->y = area.y;
            outRect->height = desiredSize.height;
            break;
        case UIVAlignment::Center:
            outRect->y = area.y + ((area.height - desiredSize.height) / 2);
            outRect->height = desiredSize.height;
            break;
        case UIVAlignment::Bottom:
            outRect->y = area.y + (area.height - desiredSize.height);
            outRect->height = desiredSize.height;
            break;
        case UIVAlignment::Stretch:
            outRect->y = area.y;
            outRect->height = area.height;
            break;
        default:
            LN_UNREACHABLE();
            break;
        }
    }

	//static Size measureInnerSpace(const Thickness& padding, const Thickness& border, bool borderInset)
	//{
	//	Size size = padding.size();
	//	*outRect = area.makeDeflate(padding);
	//}

	template<class TUIElementList>
	//Size UIFrameLayout_staticMeasureChildrenAreaSize(const List<Ref<UIElement>>& elements, const Size& constraint)
	static Size UIFrameLayout_staticMeasureChildrenAreaSize(UILayoutContext* layoutContext, const TUIElementList& elements, const Size& constraint)
	{
		Size childMaxSize(0, 0);
		for (int i = 0; i < elements->size(); i++)
		{
			auto& child = elements->at(i);
			if (layoutContext->testLayoutEnabled(child)) {
				child->measureLayout(layoutContext, constraint);
				const Size& desiredSize = child->desiredSize();
				childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
				childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
			}
		}
		return childMaxSize;
	}

	template<class TElement, class TUIElementList>
	static Size UIFrameLayout_staticArrangeChildrenArea(
		UILayoutContext* layoutContext,
		TElement* ownerElement,
		UIHAlignment horizontalContentAlignment,
		UIVAlignment verticalContentAlignment,
		const TUIElementList& elements,
		const Rect& finalArea)
	{
		for (int i = 0; i < elements->size(); i++)
		{
			auto& child = elements->at(i);
			if (layoutContext->testLayoutEnabled(child)) {

				Rect slotRect;
				detail::LayoutHelper::adjustAlignment(finalArea, child->desiredSize(), horizontalContentAlignment, verticalContentAlignment, &slotRect);

				child->arrangeLayout(layoutContext, slotRect);
			}
		}
		return finalArea.getSize();
	}

	static Rect makePaddingRect(const UIElement* element, const Size& clientSize);
};

} // namespace detail
} // namespace ln

