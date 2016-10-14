
#include "Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "DocumentsManager.h"
#include "DocumentElements.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// Document
//==============================================================================

//------------------------------------------------------------------------------
Document::Document()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
Document::~Document()
{
}

//------------------------------------------------------------------------------
void Document::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}


//==============================================================================
// TextElement
//==============================================================================

//------------------------------------------------------------------------------
TextElement::TextElement()
	: m_manager(nullptr)
	, m_fontDataModified(false)
{
}

//------------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//------------------------------------------------------------------------------
void TextElement::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_fontData.Family = String::GetEmpty();
	m_fontData.Size = 20;
	m_fontData.IsBold = false;
	m_fontData.IsItalic = false;
	m_fontData.IsAntiAlias = true;
	m_fontDataModified = true;
}

//------------------------------------------------------------------------------
void TextElement::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
SizeF TextElement::MeasureLayout()
{
	if (m_fontDataModified)
	{
		OnFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}

	return SizeF::Zero;
}

//------------------------------------------------------------------------------
void TextElement::ArrangeLayout(const RectF& finalLocalRect)
{

}

//------------------------------------------------------------------------------
void TextElement::Render(IDocumentsRenderer* renderer)
{
}


//==============================================================================
// Paragraph
//==============================================================================

//------------------------------------------------------------------------------
Paragraph::Paragraph()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Paragraph::~Paragraph()
{
}

//------------------------------------------------------------------------------
void Paragraph::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void Paragraph::AddInline(Inline* inl)
{
	LN_CHECK_ARG(inl != nullptr);
	m_inlines.Add(inl);
}

//------------------------------------------------------------------------------
void Paragraph::ClearInlines()
{
	m_inlines.Clear();
}

//==============================================================================
// Inline
//==============================================================================

//------------------------------------------------------------------------------
Inline::Inline()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Inline::~Inline()
{
}

//------------------------------------------------------------------------------
void Inline::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}


//==============================================================================
// Run
//==============================================================================

//------------------------------------------------------------------------------
Run::Run()
	: Inline()
{
}

//------------------------------------------------------------------------------
Run::~Run()
{
}

//------------------------------------------------------------------------------
void Run::Initialize(DocumentsManager* manager)
{
	Inline::Initialize(manager);

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = RefPtr<GlyphRun>::MakeRef();
}

//------------------------------------------------------------------------------
void Run::SetText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->SetText(text);
}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
SizeF Run::MeasureLayout()
{
	const SizeI& size = m_glyphRun->GetRenderSize();
	return SizeF((float)size.width, (float)size.height);
}

//------------------------------------------------------------------------------
void Run::ArrangeLayout(const RectF& finalLocalRect)
{

}

//------------------------------------------------------------------------------
void Run::Render(IDocumentsRenderer* renderer)
{
	renderer->OnDrawGlyphRun(m_glyphRun, Point::Zero);
}

} // namespace detail
LN_NAMESPACE_END
