
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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Run::Run(const String& text, DocumentsManager* manager)
	: Inline(manager)
	, m_text()
{
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
		// TODO: 描画先はソフトウェアビットマップもあり得る。もっと抽象的なクラスが良い。
		m_renderer.Attach(m_manager->GetGraphicsManager()->LookupTextRenderer(m_fontData));
	}

	Imaging::GlyphRun result;
	m_renderer->Measure(m_text.GetCStr(), m_text.GetLength(), &result);
	
	return result.AreaSize;
}

} // namespace Documents
} // namespace Lumino

