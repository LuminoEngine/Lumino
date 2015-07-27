
#include "../Internal.h"
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Documents/Run.h>

namespace Lumino
{
namespace Documents
{

//=============================================================================
// Run
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Run, Inline);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Run::Run(DocumentsManager* manager)
	: Inline(manager)
{
	m_glyphRun.Attach(LN_NEW Graphics::GlyphRun());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Run::Run(const String& text, DocumentsManager* manager)
	: Inline(manager)
	, m_text()
{
	m_glyphRun.Attach(LN_NEW Graphics::GlyphRun());
	SetText(text);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Run::~Run()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Run::SetText(const String& text)
{
	m_text.Clear();
	m_text.Append(m_manager->GetTCharToUTF32Converter()->Convert(text.GetCStr(), text.GetByteCount()));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size Run::Measure()
{
	if (m_fontDataModified)
	{
		UpdateFontData();
		m_glyphRun->m_glyphTextureCache.Attach(m_manager->GetGraphicsManager()->LookupGlyphTextureCache(m_fontData));
	}

	m_glyphRun->m_glyphTextureCache->Measure(m_text.GetCStr(), m_text.GetLength(), &m_glyphRun->m_glyphData);
	
	return m_glyphRun->m_glyphData.AreaSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Run::Render(DocumentsRenderer* renderer)
{
	renderer->OnDrawGlyphRun(m_glyphRun, Point::Zero);	// TODO: ˆÊ’u
}

} // namespace Documents
} // namespace Lumino

