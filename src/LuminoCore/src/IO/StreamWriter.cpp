
#include "Internal.hpp"
#include <Lumino/IO/Path.hpp>
#include <Lumino/IO/FileStream.hpp>
#include <Lumino/IO/StreamWriter.hpp>

namespace ln {

//==============================================================================
// StreamWriter
//==============================================================================

//------------------------------------------------------------------------------
StreamWriter::StreamWriter(Stream* stream, TextEncoding* encoding)
{
	init(stream, encoding);
}

//------------------------------------------------------------------------------
StreamWriter::StreamWriter(const Path& filePath, TextEncoding* encoding, FileWriteMode mode)
{
	// モード選択
	FileOpenMode openMode;
	if (mode == FileWriteMode::Truncate) {
		openMode = FileOpenMode::write | FileOpenMode::Truncate;
	}
	else {
		openMode = FileOpenMode::write | FileOpenMode::append;
	}

	Ref<FileStream> stream = FileStream::create(filePath.c_str(), openMode);
	init(stream, encoding);
}

//------------------------------------------------------------------------------
StreamWriter::~StreamWriter()
{
}

//------------------------------------------------------------------------------
void StreamWriter::init(Stream* stream, TextEncoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == NULL) {
		encoding = TextEncoding::utf8Encoding();
	}

	m_stream = stream;
	setEncoding(encoding);
}

//------------------------------------------------------------------------------
void StreamWriter::flash()
{
	m_stream->flush();
}

//------------------------------------------------------------------------------
void StreamWriter::writeOverride(const void* data, size_t byteCount)
{
	m_stream->write(data, byteCount);
}

} // namespace ln
