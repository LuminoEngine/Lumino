
#pragma once
#include "UIPanel.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UILayoutRoot
	: public UIPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:

LN_INTERNAL_ACCESS:
	UILayoutRoot();
	virtual ~UILayoutRoot();
	void Initialize(detail::UIManager* manager);

private:
};

LN_NAMESPACE_END
