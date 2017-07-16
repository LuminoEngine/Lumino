
#pragma once
#include "UITextElement.h"

LN_NAMESPACE_BEGIN
class UITextBlock;
using UITextBlockPtr = Ref<UITextBlock>;

/**
	@brief		
*/
class UITextBlock
	: public UITextElement
{
	LN_OBJECT;
public:

	static UITextBlockPtr create();

public:
	void setText(const StringRef& text);


LN_PROTECTED_INTERNAL_ACCESS:
	UITextBlock();
	virtual ~UITextBlock();
	void initialize();

	virtual Size measureOverride(const Size& availableSize) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(DrawingContext* g) override;

private:
	String	m_text;
};

LN_NAMESPACE_END
