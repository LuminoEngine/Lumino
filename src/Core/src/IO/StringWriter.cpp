
#include "Internal.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/IO/StringWriter.hpp>

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

void StringWriter::onWriteOverride(const void* data, size_t byteCount)
{
    if (data && byteCount > 0) {
        auto* begin = (const byte_t*)data;
        auto* end = begin + byteCount;
        m_builder.insert(m_builder.end(), begin, end);
    }
}

void StringWriter::onFlush()
{
}

} // namespace ln
