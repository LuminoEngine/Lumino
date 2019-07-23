#pragma once
#include "UIControl.hpp"

namespace ln {

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

    void adjustActualSize()
    {
        actualSize = Math::clamp(actualSize, minSize, maxSize);
    }
};

} // namespace detail


class UISplitter
	: public UIControl
{
public:
    UISplitter();
	void init();

    void addCellDefinition(UILayoutLengthType type = UILayoutLengthType::Ratio, float size = 1.0f, float minSize = 0.0f, float maxSize = FLT_MAX);

protected:
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
    Orientation m_orientation = Orientation::Horizontal;
    List<detail::GridDefinitionData> m_cellDefinitions;
};

} // namespace ln

