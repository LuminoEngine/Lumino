
#include "Internal.hpp"
#include <LuminoCore/IO/StringReader.hpp>

namespace ln {

//==============================================================================
// StringReader

StringReader::StringReader(const String& str)
    : m_src(str)
    , m_range(m_src)
    , m_pos(0)
{
}

StringReader::StringReader(const Char* str)
    : m_src(str)
    , m_range(m_src)
    , m_pos(0)
{
}

StringReader::StringReader(const StringView& str)
    : m_src()
    , m_range(str)
    , m_pos(0)
{
}

StringReader::~StringReader()
{
}

int StringReader::peek()
{
    if (m_pos >= m_range.length()) {
        return -1;
    }
    return m_range[m_pos];
}

int StringReader::read()
{
    if (m_pos >= m_range.length()) {
        return -1;
    }
    return m_range[m_pos++];
}

bool StringReader::readLine(String* line)
{
    int i = m_pos;
    while (i < m_range.length()) {
        TTCHAR ch = m_range[i];
        if (ch == '\r' || ch == '\n') {
            if (line) {
                *line = m_range.substr(m_pos, i - m_pos);
            }
            m_pos = i + 1;
            if (ch == '\r' && m_pos < m_range.length() && m_range[m_pos] == '\n') m_pos++;
            return true;
        }
        i++;
    }

    // i は EOF にたどり着いた。そして、読み取る文字があれば読む。
    if (i > m_pos) {
        if (line) {
            *line = m_range.substr(m_pos, i - m_pos);
        }
        m_pos = i;
        return true;
    }

    // EOF にたどり着いたが、読み取る文字は無かった。
    return false;
}

String StringReader::readToEnd()
{
    String s;
    if (m_pos == 0) {
        s = m_range;
    } else {
        s = m_range.substr(m_pos, m_range.length() - m_pos);
    }
    m_pos = m_range.length();
    return s;
}

bool StringReader::isEOF()
{
    return (m_pos >= m_range.length());
}

} // namespace ln
