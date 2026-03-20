// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "../Base/String.hpp"
#include "TextReader.hpp"

namespace ln {

/** 文字列から読み取る TextReader を実装します。 */
class StringReader
    : public TextReader
{
public:
    /** 指定した文字列から読み取る StringReader を構築します。 */
    StringReader(const String& str);
    StringReader(const Char* str);
    StringReader(const StringView& str);

    virtual ~StringReader();

    virtual int peek() override;
    virtual int read() override;
    virtual bool readLine(String* line) override;
    virtual String readToEnd() override;
    virtual bool isEOF() override;

private:
    String m_src;
    StringView m_range;
    int m_pos;
};

} // namespace ln
