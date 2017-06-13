
#include "Internal.h"
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UITextBlock.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UITextBlock
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextBlock, UITextElement)

//------------------------------------------------------------------------------
UITextBlockPtr UITextBlock::create()
{
	auto ptr = UITextBlockPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UITextBlock::UITextBlock()
{
}

//------------------------------------------------------------------------------
UITextBlock::~UITextBlock()
{
}

//------------------------------------------------------------------------------
void UITextBlock::initialize()
{
	UITextElement::initialize();
	//SetHAlignment(HAlignment::Center);
	//SetVAlignment(VAlignment::Center);
}

//------------------------------------------------------------------------------
void UITextBlock::setText(const StringRef& text)
{
	m_text = text;
}

//------------------------------------------------------------------------------
Size UITextBlock::measureOverride(const Size& availableSize)
{
	Size size = UITextElement::measureOverride(availableSize);

	if (m_font != nullptr)
	{
		Size textSize = m_font->measureRenderSize(m_text);
		size.width = std::max(size.width, textSize.width);
		size.height = std::max(size.height, textSize.height);
	}

	return size;
}

//------------------------------------------------------------------------------
Size UITextBlock::arrangeOverride(const Size& finalSize)
{
	return UITextElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBlock::onRender(DrawingContext* g)
{
	g->setFont(GetActiveFont());
	g->setBrush(GetForegroundInternal());
	g->DrawText_(m_text, PointF());
}

LN_NAMESPACE_END
