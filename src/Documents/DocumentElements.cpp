
#include "Internal.h"
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
	m_fontDataModified = false;
}

//------------------------------------------------------------------------------
SizeF TextElement::Measure()
{
	return SizeF::Zero;
}

//------------------------------------------------------------------------------
void TextElement::Render(IDocumentsRenderer* renderer)
{
}

} // namespace detail
LN_NAMESPACE_END
