
#pragma once
#include "UITextElement.h"

LN_NAMESPACE_BEGIN
class UITextBlock;
using UITextBlockPtr = RefPtr<UITextBlock>;

/**
	@brief		
*/
class UITextBlock
	: public UITextElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	static UITextBlockPtr Create();

public:
	void SetText(const StringRef& text);


LN_PROTECTED_INTERNAL_ACCESS:
	UITextBlock();
	virtual ~UITextBlock();
	void initialize();

	virtual Size MeasureOverride(const Size& availableSize) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRender(DrawingContext* g) override;

private:
	String	m_text;
};

LN_NAMESPACE_END
