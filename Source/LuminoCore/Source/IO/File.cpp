
#include "../Internal.h"
#include "../../include/Lumino/IO/File.h"
#include "../../include/Lumino/IO/FileSystem.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// File
//==============================================================================

//------------------------------------------------------------------------------
File::File(const String& filePath)
	: m_filePath(filePath)
	, m_fileStream(NULL)
{
}

//------------------------------------------------------------------------------
//File::File(const PathName& filePath)
//	: m_filePath(filePath)
//	, m_fileAccess(FileAccess_Read)
//	, m_stream(NULL)
//{
//}

//------------------------------------------------------------------------------
File::~File()
{
	Close();
}

//------------------------------------------------------------------------------
void File::Open(FileOpenMode openMode)
{
	LN_THROW(m_fileStream == nullptr, InvalidOperationException);	// すでにファイルが開かれている
	m_fileStream = FileStream::Create(m_filePath.c_str(), openMode);
}

//------------------------------------------------------------------------------
void File::Close()
{
	m_fileStream.SafeRelease();
}

//------------------------------------------------------------------------------
PathName File::GetFilePath() const
{
	return m_filePath.CanonicalizePath();
}

//------------------------------------------------------------------------------
String File::GetFileName() const
{
	return m_filePath.GetFileName();
}

//------------------------------------------------------------------------------
bool File::CanRead() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->CanRead();
}

//------------------------------------------------------------------------------
bool File::CanWrite() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->CanWrite();
}

//------------------------------------------------------------------------------
int64_t File::GetLength() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->GetLength();
}

//------------------------------------------------------------------------------
int64_t File::GetPosition() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->GetPosition();
}

//------------------------------------------------------------------------------
size_t File::Read(void* buffer, size_t readCount)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->Read(buffer, readCount);
}

//------------------------------------------------------------------------------
void File::Write(const void* data, size_t byteCount)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->Write(data, byteCount);
}

//------------------------------------------------------------------------------
void File::Seek(int64_t offset, SeekOrigin origin)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->Seek(offset, origin);
}

//------------------------------------------------------------------------------
void File::Flush()
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->Flush();
}


//==============================================================================
// File
//==============================================================================

//------------------------------------------------------------------------------
TemporaryFile::TemporaryFile()
	: File(PathName::GetUniqueFilePathInDirectory(PathName::GetSpecialFolderPath(SpecialFolder::Temporary), nullptr, nullptr))
	, m_autoRemove(true)
{
}

//------------------------------------------------------------------------------
TemporaryFile::~TemporaryFile()
{
	Close();

	if (m_autoRemove)
	{
		FileSystem::Delete(GetFilePath());
	}
}

//------------------------------------------------------------------------------
void TemporaryFile::Open()
{
	File::Open(FileOpenMode::ReadWrite | FileOpenMode::Truncate);
}

LN_NAMESPACE_END
