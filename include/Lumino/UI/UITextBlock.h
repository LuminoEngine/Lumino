
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
	LN_UI_TYPEINFO_DECLARE();
public:

	static UITextBlockPtr Create();

protected:
	UITextBlock();
	virtual ~UITextBlock();
	void Initialize(detail::UIManager* manager);

	virtual SizeF MeasureOverride(const SizeF& availableSize) override;
	virtual void OnRender(GraphicsContext* g) override;

private:
};

LN_NAMESPACE_END
