
#include "../Internal.h"
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Documents/TextElement.h>

namespace Lumino
{
namespace Documents
{

//=============================================================================
// TextElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextElement, CoreObject);

LN_PROPERTY_IMPLEMENT(TextElement, String, FontFamily, m_fontData.Family, String::GetEmpty());
LN_PROPERTY_IMPLEMENT(TextElement, int, FontSize, m_fontData.Size, 20);
LN_PROPERTY_IMPLEMENT(TextElement, int, FontEdgeSize, m_fontData.EdgeSize, 20);
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontBold, m_fontData.IsBold, false);
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontItalic, m_fontData.IsItalic, false);
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontAntiAlias, m_fontData.IsAntiAlias, true);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextElement::TextElement(DocumentsManager* manager)
	: m_manager(manager)
	, m_fontDataModified(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Imaging::Font* TextElement::GetFont() const
//{
//	if (m_font != NULL) { return m_font; }
//	if (m_parent != NULL) { return m_parent->GetFont(); }
//	LN_THROW(0, InvalidOperationException);
//	return NULL;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size TextElement::Measure()
{
	return Size::Zero;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextElement::Render(DocumentsRenderer* renderer)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextElement::UpdateFontData()
{
	if (m_fontDataModified)
	{
		if (m_parent != NULL)
		{
			if (!HasLocalPropertyValue(Properties::FontFamily)) { m_fontData.Family = m_parent->m_fontData.Family; }
			if (!HasLocalPropertyValue(Properties::FontSize)) { m_fontData.Size = m_parent->m_fontData.Size; }
			if (!HasLocalPropertyValue(Properties::FontEdgeSize)) { m_fontData.EdgeSize = m_parent->m_fontData.EdgeSize; }
			if (!HasLocalPropertyValue(Properties::IsFontBold)) { m_fontData.IsBold = m_parent->m_fontData.IsBold; }
			if (!HasLocalPropertyValue(Properties::IsFontItalic)) { m_fontData.IsItalic = m_parent->m_fontData.IsItalic; }
			if (!HasLocalPropertyValue(Properties::IsFontAntiAlias)) { m_fontData.IsAntiAlias = m_parent->m_fontData.IsAntiAlias; }
		}
		m_fontDataModified = true;
	}
}

} // namespace Documents
} // namespace Lumino

