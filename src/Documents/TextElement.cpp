
#include "../Internal.h"
#include <Lumino/Documents/TextElement.h>

namespace Lumino
{
namespace Documents
{

//=============================================================================
// TextElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextElement, CoreObject);

LN_DEFINE_PROPERTY_2(TextElement, String, FontFamilyProperty, "FontFamily", String::GetEmpty(), NULL, NULL);
LN_DEFINE_PROPERTY_2(TextElement, int, FontSizeProperty, " FontSize", 0, NULL, NULL);
LN_DEFINE_PROPERTY_2(TextElement, int, FontEdgeSizeProperty, "FontEdgeSize", 0, NULL, NULL);
LN_DEFINE_PROPERTY_2(TextElement, bool, IsFontBoldProperty, "IsFontBold", false, NULL, NULL);
LN_DEFINE_PROPERTY_2(TextElement, bool, IsFontItalicProperty, "IsFontItalic", false, NULL, NULL);
LN_DEFINE_PROPERTY_2(TextElement, bool, IsFontAntiAliasProperty, "IsFontAntiAlias", false, NULL, NULL);

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

} // namespace Documents
} // namespace Lumino

