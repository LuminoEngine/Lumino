
#include "Internal.h"
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/Rendering.h>
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
void UITextBlock::OnRender(DrawList* g)
{
	g->SetFont(GetActiveFont());
	g->SetBrush(GetForegroundInternal());
	g->DrawText_(m_text, PointF::Zero);
}

LN_NAMESPACE_END
