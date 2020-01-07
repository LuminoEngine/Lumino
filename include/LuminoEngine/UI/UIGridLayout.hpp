#pragma once
#include "UILayoutPanel.hpp"

namespace ln {

class UIGridLayout
    : public UILayoutPanel2
{
public:

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIGridLayout();
	void init();

private:
	List<detail::GridDefinitionData> m_rowDefinitions;
	List<detail::GridDefinitionData> m_columnDefinitions;
};

} // namespace ln

