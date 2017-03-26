
#include "Internal.h"
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Documents/Documents.h>
#include <Lumino/UI/UITextBox.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UITextBox
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextBox, UITextElement)

//------------------------------------------------------------------------------
UITextBoxPtr UITextBox::Create()
{
	auto ptr = RefPtr<UITextBox>::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UITextBox::UITextBox()
{
}

//------------------------------------------------------------------------------
UITextBox::~UITextBox()
{
}

//------------------------------------------------------------------------------
void UITextBox::Initialize(detail::UIManager* manager)
{
	UITextElement::Initialize(manager);
	//SetHAlignment(HAlignment::Center);
	//SetVAlignment(VAlignment::Center);

	m_document = NewObject<tr::Document>();
	m_documentView = NewObject<tr::DocumentView>(m_document);
}

//------------------------------------------------------------------------------
void UITextBox::SetText(const StringRef& text)
{
	m_document->SetText(text);
}

//------------------------------------------------------------------------------
Size UITextBox::MeasureOverride(const Size& availableSize)
{
	Size size = UITextElement::MeasureOverride(availableSize);

	if (m_font != nullptr)
	{
		//Size textSize = m_font->MeasureRenderSize(m_text);
		//size.width = std::max(size.width, textSize.width);
		//size.height = std::max(size.height, textSize.height);
	}

	m_documentView->MeasureLayout(availableSize);

	return size;
}

//------------------------------------------------------------------------------
Size UITextBox::ArrangeOverride(const Size& finalSize)
{
	m_documentView->ArrangeLayout(RectF(0, 0, finalSize));

	return UITextElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBox::OnRender(DrawList* g)
{
	UITextElement::OnRender(g);

	g->SetFont(GetActiveFont());	// TODO:
	m_documentView->Render(g);
	//g->SetFont(GetActiveFont());
	//g->SetBrush(GetForegroundInternal());
	//g->DrawText_(m_text, PointF::Zero);
	g->DrawChar('g', PointF(0, 0));
}

LN_NAMESPACE_END
