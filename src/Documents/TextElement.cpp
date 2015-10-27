
#include "../Internal.h"
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Documents/TextElement.h>

LN_NAMESPACE_BEGIN
namespace Documents
{

//=============================================================================
// TextElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextElement, CoreObject);

LN_PROPERTY_IMPLEMENT(TextElement, String, FontFamilyProperty, "FontFamily", m_fontData.Family, PropertyMetadata(String::GetEmpty()));
LN_PROPERTY_IMPLEMENT(TextElement, int, FontSizeProperty, "FontSize", m_fontData.Size, PropertyMetadata(20));
LN_PROPERTY_IMPLEMENT(TextElement, int, FontEdgeSizeProperty, "FontEdgeSize", m_fontData.EdgeSize, PropertyMetadata(20));
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontBoldProperty, "IsFontBold", m_fontData.IsBold, PropertyMetadata(false));
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontItalicProperty, "IsFontItalic", m_fontData.IsItalic, PropertyMetadata(false));
LN_PROPERTY_IMPLEMENT(TextElement, bool, IsFontAntiAliasProperty, "IsFontAntiAlias", m_fontData.IsAntiAlias, PropertyMetadata(true));

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
			if (!HasLocalPropertyValue(FontFamilyProperty)) { m_fontData.Family = m_parent->m_fontData.Family; }
			if (!HasLocalPropertyValue(FontSizeProperty)) { m_fontData.Size = m_parent->m_fontData.Size; }
			if (!HasLocalPropertyValue(FontEdgeSizeProperty)) { m_fontData.EdgeSize = m_parent->m_fontData.EdgeSize; }
			if (!HasLocalPropertyValue(IsFontBoldProperty)) { m_fontData.IsBold = m_parent->m_fontData.IsBold; }
			if (!HasLocalPropertyValue(IsFontItalicProperty)) { m_fontData.IsItalic = m_parent->m_fontData.IsItalic; }
			if (!HasLocalPropertyValue(IsFontAntiAliasProperty)) { m_fontData.IsAntiAlias = m_parent->m_fontData.IsAntiAlias; }
		}
		m_fontDataModified = false;
	}
}

} // namespace Documents
LN_NAMESPACE_END

