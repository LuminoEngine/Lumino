
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
	return m_builder.toString();
}

//------------------------------------------------------------------------------
void StringWriter::flash()
{
}

//------------------------------------------------------------------------------
void StringWriter::writeOverride(const void* data, size_t byteCount)
{
	m_builder.append((const TCHAR*)data, byteCount / sizeof(TCHAR));
}

LN_NAMESPACE_END
