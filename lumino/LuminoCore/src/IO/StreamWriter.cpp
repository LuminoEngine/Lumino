// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#include "Internal.hpp"
#include <LuminoCore/IO/Path.hpp>
#include <LuminoCore/IO/FileStream.hpp>
#include <LuminoCore/IO/StreamWriter.hpp>

namespace ln {

//==============================================================================
// StreamWriter

StreamWriter::StreamWriter(Stream* stream, TextEncoding* encoding)
{
    init(stream, encoding);
}

StreamWriter::StreamWriter(const Path& filePath, FileWriteMode mode, TextEncoding* encoding)
{
    FileOpenMode openMode;
    if (mode == FileWriteMode::Truncate) {
        openMode = FileOpenMode::Write | FileOpenMode::Truncate;
    } else {
        openMode = FileOpenMode::Write | FileOpenMode::Append;
    }

    Ref<FileStream> stream = FileStream::create(filePath.c_str(), openMode);
    init(stream, encoding);
}

StreamWriter::~StreamWriter()
{
}

void StreamWriter::init(Stream* stream, TextEncoding* encoding)
{
    if (LN_ENSURE(stream)) return;

    // encoding 未指定であれば UTF8 とする
    if (encoding == nullptr) {
        encoding = TextEncoding::utf8Encoding();
    }

    m_stream = stream;
    setEncoding(encoding);
}

void StreamWriter::onWriteOverride(const void* data, size_t byteCount)
{
    if (data && byteCount > 0) {
        m_stream->write(data, byteCount);
    }
}

void StreamWriter::onFlush()
{
    m_stream->flush();
}

} // namespace ln
