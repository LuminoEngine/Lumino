
#include <float.h>
#include "Internal.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/IO/TextWriter.hpp>

namespace ln {

//==============================================================================
// TextWriter

TextWriter::TextWriter()
    : m_newLine(String::newLine())
    , m_locale()
    , m_writtenPreamble(true)
{
    m_converter.getSourceEncoding(TextEncoding::tcharEncoding());
    m_converter.setDestinationEncoding(TextEncoding::tcharEncoding());
}

TextWriter::~TextWriter()
{
}

void TextWriter::write(char value)
{
    Char ch = value;
    writeInternal(&ch, 1);
}

void TextWriter::write(wchar_t value)
{
    Char ch = value;
    writeInternal(&ch, 1);
}

void TextWriter::write(char16_t value)
{
    Char ch = value;
    writeInternal(&ch, 1);
}

void TextWriter::write(char32_t value)
{
    Char ch = value;
    writeInternal(&ch, 1);
}

void TextWriter::write(int16_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::int64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}
void TextWriter::write(int32_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::int64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}
void TextWriter::write(int64_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::int64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}

void TextWriter::write(uint16_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::uint64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}
void TextWriter::write(uint32_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::uint64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}

void TextWriter::write(uint64_t value)
{
    char buf1[64];
    Char buf2[64];
    int len = StringHelper::uint64ToString(value, 'D', buf1, 64);
    StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
    writeInternal(buf2, len);
}

void TextWriter::write(float value)
{
    write((double)value);
}

void TextWriter::write(double value)
{
    if (value < FLT_MIN || FLT_MAX < value) {
        std::vector<char> buf1;
        std::vector<Char> buf2;
        buf1.resize(512);
        buf2.resize(512);
        int len = StringHelper::doubleToString(value, 'F', -1, buf1.data(), static_cast<int>(buf1.size()));
        StringHelper::copySimpleAsciiString(buf2.data(), len, buf1.data(), len);
        writeInternal(buf2.data(), len);
    } else {
        char buf1[64];
        Char buf2[64];
        int len = StringHelper::doubleToString(value, 'F', -1, buf1, 64);
        StringHelper::copySimpleAsciiString(buf2, len, buf1, len);
        writeInternal(buf2, len);
    }
}

void TextWriter::write(const StringView& str)
{
    writeInternal(str.data(), str.length());
}

void TextWriter::writeLine()
{
    writeInternal(m_newLine.c_str(), m_newLine.length());
}

void TextWriter::writeLine(char value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(wchar_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(char16_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(char32_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(int16_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(int32_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(int64_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(uint16_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(uint32_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(uint64_t value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(float value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(double value)
{
    write(value);
    writeLine();
}

void TextWriter::writeLine(const StringView& str)
{
    writeInternal(str.data(), str.length());
    writeLine();
}

void TextWriter::flush()
{
    onFlush();
}

void TextWriter::setEncoding(TextEncoding* encoding)
{
    m_converter.setDestinationEncoding(encoding);
}

TextEncoding* TextWriter::encoding() const
{
    return m_converter.getDestinationEncoding();
}

void TextWriter::setNewLine(const String& newLine)
{
    m_newLine = newLine;
}

const String& TextWriter::newLine() const
{
    return m_newLine;
}

void TextWriter::setFormatLocale(const Locale& locale)
{
    m_locale = locale;
}

const Locale& TextWriter::formatLocale() const
{
    return m_locale;
}

void TextWriter::writeInternal(const Char* str, int len)
{
    // write BOM if needed
    if (!m_writtenPreamble) {
        const byte_t* bom = m_converter.getDestinationEncoding()->preamble();
        size_t len = strlen((char*)bom);
        if (len > 0) {
            onWriteOverride(bom, len);
        }
        m_writtenPreamble = true;
    }

    if (str == nullptr || len == 0) {
        return;
    }

    const ByteBuffer& buf = m_converter.convert(str, len * sizeof(Char));
    onWriteOverride(buf.data(), buf.size());
}

} // namespace ln
