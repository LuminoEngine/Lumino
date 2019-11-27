#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "../Graphics/ColorStructs.hpp"
#include "../Rendering/Common.hpp"
#include "Common.hpp"

namespace ln {
class Font;
class Texture;
class Shader;
class AbstractMaterial;
class UIStyle;
class UILayoutContext;
class UIElement;
namespace detail { class LayoutHelper; }


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
enum class VAlignment
{
	Top = 0,			/**< 子要素を、親のレイアウト スロットの上端に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Bottom,				/**< 子要素を、親のレイアウト スロットの下端に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
};

/** 横方向の表示位置を示します。*/
enum class HAlignment
{
	Left = 0,			/**< 子要素を、親のレイアウト スロットの左側に揃えて配置します。*/
	Center,				/**< 子要素を、親のレイアウト スロットの中央に揃えて配置します。*/
	Right,				/**< 子要素を、親のレイアウト スロットの右側に揃えて配置します。*/
	Stretch,			/**< 子要素を、親のレイアウト スロット全体に引き伸ばします。*/
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
//	HAlignment horizontalAlignment;
//	VAlignment verticalAlignment;
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
//	Ref<AbstractMaterial> backgroundMaterial;
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

    const Size& actualSize() const { return m_actualSize; }

	//const Rect& finalGlobalRect() const { return m_finalGlobalRect; }

    // TODO: internal
    const Point localPosition() const { return m_localPosition; }

protected:
	virtual ~UILayoutElement();

	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint);
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize);

	//virtual int getChildLayoutItemCount() const = 0;
	//virtual ILayoutElement* getChildLayoutItem(int index) const = 0;


	virtual void onUpdateLayout(UILayoutContext* layoutContext);


public:	//TODO: internal
	void measureLayout(UILayoutContext* layoutContext, const Size& availableSize);
	void arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect);
	///*virtual*/ void updateFinalRects(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect);
    void updateFinalRects(UILayoutContext* layoutContext, const Matrix& parentCombinedRenderTransform);

	//const Size& getLayoutSize() const { return m_layoutSize; }
	void setLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
	const Size& getLayoutDesiredSize() const { return m_desiredSize; }
	const Thickness& getLayoutMargin() const;
	const Thickness& getLayoutPadding() const;
	HAlignment getLayoutHAlignment() const;
	VAlignment getLayoutVAlignment() const;
	void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const;

	//void setLayoutFinalLocalRect(const Rect& rect) { m_finalLocalRect = rect; }
	//const Rect& getLayoutFinalLocalRect() const { return m_finalLocalRect; }
	//void setLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }

	//Size m_layoutSize;
	const detail::UIStyleInstance* m_finalStyle;
	Size m_desiredSize; // includes, margin, border

    

    Matrix m_combinedFinalRenderTransform;

private:
    Point m_localPosition;  // 親コンテナ内の相対座標
    Size m_actualSize;
    //Rect m_finalGlobalRect; // TODO: obsolete



	friend class detail::LayoutHelper;
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
	static Rect arrangeClientArea(UILayoutElement* element, const Size& finalSize);

	//static void forEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	//{
	//	int count = element->getVisualChildrenCount();
	//	for (int i = 0; i < count; ++i)
	//	{
	//		func(element->getVisualChild(i));
	//	}
	//}

	// widthNan : ユーザーが希望するサイズを指定しているか
	static void adjustHorizontalAlignment(const Size& areaSize, const Size& desiredSize, float fixedSizeOrNaN, HAlignment align, Rect* outRect)
	{
		switch (align)
		{
		case HAlignment::Left:
			outRect->x = 0;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Center:
			outRect->x = (areaSize.width - desiredSize.width) / 2;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Right:
			outRect->x = areaSize.width - desiredSize.width;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Stretch:
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

	static void adjustVerticalAlignment(const Size& areaSize, const Size& desiredSize, float fixedSizeOrNaN, VAlignment align, Rect* outRect)
	{
		switch (align)
		{
		case VAlignment::Top:
			outRect->y = 0;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Center:
			outRect->y = (areaSize.height - desiredSize.height) / 2;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Bottom:
			outRect->y = areaSize.height - desiredSize.height;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Stretch:
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

    static void adjustAlignment(const Rect& area, const Size& desiredSize, HAlignment halign, VAlignment valign, Rect* outRect)
    {
        assert(!Math::isNaNOrInf(area.width));
        assert(!Math::isNaNOrInf(area.height));
        assert(!Math::isNaNOrInf(desiredSize.width));
        assert(!Math::isNaNOrInf(desiredSize.height));
        assert(outRect);

        switch (halign)
        {
        case HAlignment::Left:
            outRect->x = 0;
            outRect->width = desiredSize.width;
            break;
        case HAlignment::Center:
            outRect->x = (area.width - desiredSize.width) / 2;
            outRect->width = desiredSize.width;
            break;
        case HAlignment::Right:
            outRect->x = area.width - desiredSize.width;
            outRect->width = desiredSize.width;
            break;
        case HAlignment::Stretch:
            outRect->x = 0;
            outRect->width = area.width;
            break;
        default:
            LN_UNREACHABLE();
            break;
        }

        switch (valign)
        {
        case VAlignment::Top:
            outRect->y = 0;
            outRect->height = desiredSize.height;
            break;
        case VAlignment::Center:
            outRect->y = (area.height - desiredSize.height) / 2;
            outRect->height = desiredSize.height;
            break;
        case VAlignment::Bottom:
            outRect->y = area.height - desiredSize.height;
            outRect->height = desiredSize.height;
            break;
        case VAlignment::Stretch:
            outRect->y = 0;
            outRect->height = area.height;
            break;
        default:
            LN_UNREACHABLE();
            break;
        }

        outRect->x += area.x;
        outRect->y += area.y;
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
			child->measureLayout(layoutContext, constraint);
			const Size& desiredSize = child->desiredSize();
			childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
			childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
		}
		return childMaxSize;
	}

	template<class TElement, class TUIElementList>
	static Size UIFrameLayout_staticArrangeChildrenArea(UILayoutContext* layoutContext, TElement* ownerElement, const TUIElementList& elements, const Rect& finalArea)
	{
		for (int i = 0; i < elements->size(); i++)
		{
			auto& child = elements->at(i);

			Rect slotRect;
			detail::LayoutHelper::adjustAlignment(finalArea, child->desiredSize(), ownerElement->m_finalStyle->horizontalContentAlignment, ownerElement->m_finalStyle->verticalContentAlignment, &slotRect);

			child->arrangeLayout(layoutContext, slotRect);
		}
		return finalArea.getSize();
	}

	static Rect makePaddingRect(const UIElement* element, const Size& clientSize);
};

} // namespace detail
} // namespace ln

