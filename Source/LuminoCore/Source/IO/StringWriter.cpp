
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
String StringWriter::ToString() const
{
	return m_builder.ToString();
}

//------------------------------------------------------------------------------
void StringWriter::Flash()
{
}

//------------------------------------------------------------------------------
void StringWriter::WriteOverride(const void* data, size_t byteCount)
{
	m_builder.Append((const TCHAR*)data, byteCount / sizeof(TCHAR));
}

LN_NAMESPACE_END
