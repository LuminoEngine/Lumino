// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "Common.hpp"
#include "TextWriter.hpp"
#include "IOError.hpp"

namespace ln {
class Path;

/**
 * 文字列を特定のエンコーディングでストリームに書き込むための TextWriter の実装です。
 */
class StreamWriter : public TextWriter {
public:
    /**
     * 指定したパスのファイルへ書き込む StreamWriter を構築します。
     *
     * @param[in] filePath    : 書き込み先ファイルのパス
     * @param[in] mode        : ファイルのオープンモード
     * @param[in] encoding    : 書き込み時に使用するエンコーディング
     *
     * encoding を省略した場合は UTF-8 文字列として書き込みます。
     */
    static IOResult<Ref<StreamWriter>> open(const Path& filePath, FileWriteMode mode = FileWriteMode::Truncate, TextEncoding* encoding = nullptr);

    /**
     * 指定したストリームへ書き込む StreamWriter を構築します。
     *
     * @param[in] stream    : 書き込み先ストリーム
     * @param[in] encoding    : 書き込み時に使用するエンコーディング
     *
     * encoding を省略した場合は UTF-8 文字列として書き込みます。
     */
    static Ref<StreamWriter> create(Stream* stream, TextEncoding* encoding = nullptr);
    
    virtual ~StreamWriter();

private:
    StreamWriter(Stream* stream, TextEncoding* encoding);
    void onWriteOverride(const void* data, size_t byteCount) override;
    void onFlush() override;

    Ref<Stream> m_stream;
};

} // namespace ln
