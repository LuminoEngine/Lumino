
#include "Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/UI/UITextBlock.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// UITextBlock
//=============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextBlock, UITextElement)

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextBlockPtr UITextBlock::Create()
{
	auto ptr = UITextBlockPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextBlock::UITextBlock()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextBlock::~UITextBlock()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextBlock::Initialize(detail::UIManager* manager)
{
	UITextElement::Initialize(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UITextBlock::MeasureOverride(const SizeF& availableSize)
{
	SizeF size = UITextElement::MeasureOverride(availableSize);
	//Size parSize = m_paragraph->Measure();

	//size.Width = (float)std::max(parSize.Width, parSize.Width);
	//size.Height = (float)std::max(parSize.Height, parSize.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextBlock::OnRender(GraphicsContext* g)
{
	g->SetBrush(ColorBrush::Blue);
	g->SetFont(GetActiveFont());
	g->DrawText(_T("UITextBlock"), PointF::Zero);
}

LN_NAMESPACE_END
