
#include "Internal.hpp"
#include <Lumino/Base/String.hpp>
#include <Lumino/IO/StringWriter.hpp>

namespace ln {

//==============================================================================
// StringWriter

StringWriter::StringWriter()
{
}

StringWriter::~StringWriter()
{
}

String StringWriter::toString() const
{
    return String((const Char*)m_builder.data(), m_builder.size() / sizeof(Char));
}

void StringWriter::flash()
{
}

void StringWriter::writeOverride(const void* data, size_t byteCount)
{
    auto* begin = (const byte_t*)data;
    auto* end = begin + byteCount;
    m_builder.insert(m_builder.end(), begin, end);
}

} // namespace ln
