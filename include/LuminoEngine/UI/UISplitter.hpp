#pragma once
#include "UIControl.hpp"

namespace ln {
class UIThumb;

/** グリッドレイアウトのセルサイズを指定する値の種類です。*/
enum class UILayoutLengthType
{
    Auto,				/**< 子要素のサイズに合わせる */
    Direct,				/**< サイズを直接指定する */
    Ratio,				/**< レイアウト後、残りの領域を使う */
};

namespace detail {
struct GridDefinitionData
{
    // input data
    UILayoutLengthType	type = UILayoutLengthType::Ratio;
    float			size = 0.0f;
    float			minSize = 0.0f;
    float			maxSize = FLT_MAX;

    // working data
    float			desiredSize = 0.0f;
    float			actualOffset = 0.0f;	// 最終オフセット
    float			actualSize = 0.0f;		// 最終サイズ

    float getAvailableDesiredSize() const
    {
        if (type == UILayoutLengthType::Auto) {
            return desiredSize;
        }
        else if (type == UILayoutLengthType::Direct) {
            return Math::clamp(size, minSize, maxSize);
        }
        else {
            return 0;
        }
    }

    float getRatioSize() const
    {
        return (size == 0.0f) ? 1.0f : size;
    }
};

} // namespace detail


class UISplitter
	: public UIControl
{
public:
    static const int ThumbWidth = 5;

    UISplitter();
	void init();

	void setOrientation(Orientation value);
    void setCellDefinition(int index, UILayoutLengthType type = UILayoutLengthType::Ratio, float size = 1.0f, float minSize = 0.0f, float maxSize = FLT_MAX);
    void resetCellSize(int index);

protected:
    virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
	bool isHorizontal() const { return m_orientation == Orientation::Horizontal || m_orientation == Orientation::ReverseHorizontal; }

	struct CellDefinition
	{
		// input data (initial data)
		UILayoutLengthType type = UILayoutLengthType::Ratio;
		float size = 1.0f;
		float minSize = 0.0f;
		float maxSize = FLT_MAX;

		// working data
		float desiredSize = Math::NaN;
		float actualOffset = Math::NaN;
		float actualSize = Math::NaN;
	};

    Orientation m_orientation = Orientation::Horizontal;
    List<CellDefinition> m_cellDefinitions;
    List<Ref<UIThumb>> m_thumbs;
	float m_dragStartSize1;
	float m_dragStartSize2;
};

} // namespace ln

