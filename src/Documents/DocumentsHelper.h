
#pragma once
#include <Lumino/Documents/TextElement.h>

LN_NAMESPACE_BEGIN
namespace Documents
{

class DocumentsHelper
{
public:
	static void TextElement_SetParent(TextElement* obj, TextElement* value) { obj->m_parent = value; }
};


} // namespace Documents
LN_NAMESPACE_END
