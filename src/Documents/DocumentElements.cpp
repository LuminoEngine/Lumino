
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
void TextElement::OnFontDataChanged(const GraphicsManager::FontData& newData)
{
}

//------------------------------------------------------------------------------
SizeF TextElement::Measure()
{
	if (m_fontDataModified)
	{
		OnFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}

	return SizeF::Zero;
}

//------------------------------------------------------------------------------
void TextElement::Render(IDocumentsRenderer* renderer)
{
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
}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const GraphicsManager::FontData& newData)
{
	//m_glyphRun->
	//RefPtr<Internal::FontGlyphTextureCache> cache(GetManager()->GetGraphicsManager()->LookupGlyphTextureCache(newData), false);
	//Helper::AttachGlyphTextureCache(m_glyphRun, cache);
}

//------------------------------------------------------------------------------
SizeF Run::Measure()
{
	return SizeF::Zero;
}

//------------------------------------------------------------------------------
void Run::Render(IDocumentsRenderer* renderer)
{
}

} // namespace detail
LN_NAMESPACE_END
