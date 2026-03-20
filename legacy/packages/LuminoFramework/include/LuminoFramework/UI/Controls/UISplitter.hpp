#pragma once
#include "UIControl.hpp"
#include "../Layout/UILayoutPanel.hpp"

namespace ln {
class UIThumb;


/**
 *
 * 
 * Bar のデフォルト幅は 0.
 */
class UISplitter
	: public UIControl
{
public:
    UISplitter();
	void init();

	void setOrientation(UILayoutOrientation value);

	/** セルサイズを計算するための情報を設定します。 */
    void setCellDefinition(int index, UILayoutLengthType type = UILayoutLengthType::Ratio, float size = 1.0f, float minSize = 0.0f, float maxSize = std::numeric_limits<float>::max());

	/** セルサイズの再計算を要求します。 */
    void resetCellSizes();

	/** 実際にマウスドラッグ可能なバーの幅を設定します。(default: 6) */
	void setBarGap(float value);

protected:
	UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;
    void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    void onRoutedEvent(UIEventArgs* e) override;

private:
	bool isHorizontal() const { return m_orientation == UILayoutOrientation::Horizontal || m_orientation == UILayoutOrientation::ReverseHorizontal; }
	bool findNearThumb(const Point& pos, UIThumb** outThumb, float* outDistance);

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

    UILayoutOrientation m_orientation = UILayoutOrientation::Horizontal;
	float m_gap;

    List<CellDefinition> m_cellDefinitions;
    List<Ref<UIThumb>> m_thumbs;
	float m_dragStartSize1;
	float m_dragStartSize2;

	Size m_lastArrangeFinalSize;
};

} // namespace ln

