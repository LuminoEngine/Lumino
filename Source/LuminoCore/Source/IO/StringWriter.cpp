
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
#ifdef LN_USTRING
	return m_builder.c_str();
#else
	return m_builder.toString();
#endif
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
