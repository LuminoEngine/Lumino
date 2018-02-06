
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/StringWriter.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// StringWriter
//==============================================================================

//------------------------------------------------------------------------------
StringWriter::StringWriter()
{
}

//------------------------------------------------------------------------------
StringWriter::~StringWriter()
{
}

//------------------------------------------------------------------------------
String StringWriter::toString() const
{
	return String(m_builder.c_str(), m_builder.getLength());
}

//------------------------------------------------------------------------------
void StringWriter::flash()
{
}

//------------------------------------------------------------------------------
void StringWriter::writeOverride(const void* data, size_t byteCount)
{
	m_builder.append((const Char*)data, byteCount / sizeof(Char));
}

LN_NAMESPACE_END
