
#include "../Internal.h"
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/StreamWriter.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// StreamWriter
//==============================================================================

//------------------------------------------------------------------------------
StreamWriter::StreamWriter(Stream* stream, Encoding* encoding)
{
	init(stream, encoding);
}

//------------------------------------------------------------------------------
StreamWriter::StreamWriter(const PathName& filePath, Encoding* encoding, FileWriteMode mode)
{
	// モード選択
	FileOpenMode openMode;
	if (mode == FileWriteMode_Truncate) {
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
void StreamWriter::init(Stream* stream, Encoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == NULL) {
		encoding = Encoding::getUTF8Encoding();
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

LN_NAMESPACE_END
