
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
	Init(stream, encoding);
}

//------------------------------------------------------------------------------
StreamWriter::StreamWriter(const PathName& filePath, Encoding* encoding, FileWriteMode mode)
{
	// モード選択
	FileOpenMode openMode;
	if (mode == FileWriteMode_Truncate) {
		openMode = FileOpenMode::Write | FileOpenMode::Truncate;
	}
	else {
		openMode = FileOpenMode::Write | FileOpenMode::Append;
	}

	FileStreamPtr stream = FileStream::Create(filePath, openMode);
	Init(stream, encoding);
}

//------------------------------------------------------------------------------
StreamWriter::~StreamWriter()
{
}

//------------------------------------------------------------------------------
void StreamWriter::Init(Stream* stream, Encoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == NULL) {
		encoding = Encoding::GetUTF8Encoding();
	}

	m_stream = stream;
	SetEncoding(encoding);
}

//------------------------------------------------------------------------------
void StreamWriter::Flash()
{
	m_stream->Flush();
}

//------------------------------------------------------------------------------
void StreamWriter::WriteOverride(const void* data, size_t byteCount)
{
	m_stream->Write(data, byteCount);
}

LN_NAMESPACE_END
