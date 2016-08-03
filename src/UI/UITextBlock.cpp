
#include "Internal.h"
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/UI/UITextBlock.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UITextBlock
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextBlock, UITextElement)

//------------------------------------------------------------------------------
UITextBlockPtr UITextBlock::Create()
{
	auto ptr = UITextBlockPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
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
void UITextBlock::Initialize(detail::UIManager* manager)
{
	UITextElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void UITextBlock::SetText(const StringRef& text)
{
	m_text = text;
}

//------------------------------------------------------------------------------
SizeF UITextBlock::MeasureOverride(const SizeF& availableSize)
{
	SizeF size = UITextElement::MeasureOverride(availableSize);

	if (m_font != nullptr)
	{
		SizeI textSize = m_font->GetTextSize(m_text);
		size.width = std::max(size.width, (float)textSize.width);
		size.height = std::max(size.height, (float)textSize.height);
	}

	return size;
}

//------------------------------------------------------------------------------
void UITextBlock::OnRender(DrawingContext* g)
{
	g->SetBrush(ColorBrush::Blue);
	g->SetFont(GetActiveFont());
	g->DrawText(m_text, PointF::Zero);
	g->Flush();
}

LN_NAMESPACE_END
