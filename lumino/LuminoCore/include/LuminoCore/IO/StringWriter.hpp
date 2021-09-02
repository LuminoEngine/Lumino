// Copyright (c) 2018+ lriki. Distributed under the MIT license..

#pragma once
#include <vector>
#include "TextWriter.hpp"

namespace ln {

/**	文字列に処理結果を出力する TextWriter です。 */
class StringWriter
    : public TextWriter
{
public:
    StringWriter();
    virtual ~StringWriter();

    /** 処理結果の文字列を取得します。 */
    String toString() const;

private:
    virtual void onWriteOverride(const void* data, size_t byteCount) override;
    virtual void onFlush() override;

    std::vector<byte_t> m_builder;
};

} // namespace ln
