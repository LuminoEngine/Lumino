// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#include "Internal.hpp"
#include <LuminoCore/IO/Path.hpp>
#include <LuminoCore/IO/FileStream.hpp>
#include <LuminoCore/IO/StreamWriter.hpp>

namespace ln {

//==============================================================================
// StreamWriter

IOResult<Ref<StreamWriter>> StreamWriter::open(const Path& filePath, FileWriteMode mode, TextEncoding* encoding) {
    FileOpenMode openMode;
    if (mode == FileWriteMode::Truncate) {
        openMode = FileOpenMode::Write | FileOpenMode::Truncate;
    }
    else {
        openMode = FileOpenMode::Write | FileOpenMode::Append;
    }
    auto stream = FileStream::create(filePath.c_str(), openMode);
    if (!stream) {
        return err();   // TODO: FileStream からエラーを取る
    }

    return Ref<StreamWriter>(LN_NEW StreamWriter(stream, encoding), false);
}

Ref<StreamWriter> StreamWriter::create(Stream* stream, TextEncoding* encoding) {
    if (LN_REQUIRE(stream)) return nullptr;
    return Ref<StreamWriter>(LN_NEW StreamWriter(stream, encoding), false);
}

StreamWriter::StreamWriter(Stream* stream, TextEncoding* encoding) {
    LN_DCHECK(stream);

    // encoding 未指定であれば UTF8 とする
    if (encoding == nullptr) {
        encoding = TextEncoding::utf8Encoding();
    }

    m_stream = stream;
    setEncoding(encoding);
}

StreamWriter::~StreamWriter() {
}

void StreamWriter::onWriteOverride(const void* data, size_t byteCount) {
    if (data && byteCount > 0) {
        m_stream->write(data, byteCount);
    }
}

void StreamWriter::onFlush() {
    m_stream->flush();
}

} // namespace ln
