
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
UITextBlockPtr UITextBlock::Create()
{
	auto ptr = UITextBlockPtr::MakeRef();
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
void UITextBlock::SetText(const StringRef& text)
{
	m_text = text;
}

//------------------------------------------------------------------------------
Size UITextBlock::MeasureOverride(const Size& availableSize)
{
	Size size = UITextElement::MeasureOverride(availableSize);

	if (m_font != nullptr)
	{
		Size textSize = m_font->MeasureRenderSize(m_text);
		size.width = std::max(size.width, textSize.width);
		size.height = std::max(size.height, textSize.height);
	}

	return size;
}

//------------------------------------------------------------------------------
Size UITextBlock::ArrangeOverride(const Size& finalSize)
{
	return UITextElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBlock::OnRender(DrawingContext* g)
{
	g->SetFont(GetActiveFont());
	g->SetBrush(GetForegroundInternal());
	g->DrawText_(m_text, PointF());
}

LN_NAMESPACE_END
