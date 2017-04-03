
#include "Internal.h"
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Documents/Documents.h>
#include <Lumino/UI/UITextBox.h>
#include "UIManager.h"

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Text/FontManager.h"

LN_NAMESPACE_BEGIN

class UITextDocumentLine
	//: public tr::DocumentContentElement
	: public Object
{

};

class UITextDocument
	: public tr::DocumentContentElement
{
public:
	void UITextDocument::Replace(int offset, int length, const StringRef& text);

LN_CONSTRUCT_ACCESS:
	UITextDocument();
	virtual ~UITextDocument();
	void Initialize();

private:
	void ReplaceInternal(int offset, int length, const UTF32* text, int len);

	detail::UIManager*					m_manager;
	List<RefPtr<UITextDocumentLine>>	m_lines;
};


//class UITextVisualElement
//	: public Object
//{
//
//};

class UITextVisualLine
	: public Object
{

private:
	int		m_revision;
};



class UITextArea	// TODO: AvalonDock “I‚É‚Í "View"
	: public Object
{

private:
	RefPtr<UITextDocument>			m_document;
	List<RefPtr<UITextVisualLine>>	m_visualLines;
	int								m_revision;
};

//==============================================================================
// UITextDocument
//==============================================================================

//------------------------------------------------------------------------------
UITextDocument::UITextDocument()
{
}

//------------------------------------------------------------------------------
UITextDocument::~UITextDocument()
{
}

//------------------------------------------------------------------------------
void UITextDocument::Initialize()
{
	m_manager = detail::EngineDomain::GetUIManager();
}

//------------------------------------------------------------------------------
void UITextDocument::Replace(int offset, int length, const StringRef& text)
{
	// UTF32 ‚Ö•ÏŠ·
	const ByteBuffer& utf32Buf = m_manager->GetGraphicsManager()->GetFontManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), sizeof(TCHAR) * text.GetLength());
	int len = utf32Buf.GetSize() / sizeof(UTF32);
	ReplaceInternal(offset, length, (const UTF32*)utf32Buf.GetConstData(), len);
}

//------------------------------------------------------------------------------
void UITextDocument::ReplaceInternal(int offset, int length, const UTF32* text, int len)
{

}

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
	//g->DrawChar('g', PointF(0, 0));
}

LN_NAMESPACE_END
