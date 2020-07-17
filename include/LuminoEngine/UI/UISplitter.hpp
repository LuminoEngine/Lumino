#pragma once
#include "UIControl.hpp"
#include "UILayoutPanel.hpp"

namespace ln {
class UIThumb;



class UISplitter
	: public UIControl
{
public:
    static const int ThumbWidth = 4;

    UISplitter();
	void init();

	void setOrientation(Orientation value);

	/** セルサイズを計算するための情報を設定します。 */
    void setCellDefinition(int index, UILayoutLengthType type = UILayoutLengthType::Ratio, float size = 1.0f, float minSize = 0.0f, float maxSize = std::numeric_limits<float>::max());

	/** セルサイズの再計算を要求します。 */
    void resetCellSizes();


protected:
    virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
	bool isHorizontal() const { return m_orientation == Orientation::Horizontal || m_orientation == Orientation::ReverseHorizontal; }

	struct CellDefinition
	{
		// input data (initial data)
		UILayoutLengthType type = UILayoutLengthType::Ratio;
		float size = 1.0f;
		float minSize = 0.0f;
		float maxSize = std::numeric_limits<float>::max();

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

	Size m_lastArrangeFinalSize;
};

} // namespace ln

