
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
	close();
}

//------------------------------------------------------------------------------
void File::open(FileOpenMode openMode)
{
	LN_THROW(m_fileStream == nullptr, InvalidOperationException);	// すでにファイルが開かれている
	m_fileStream = FileStream::create(m_filePath.c_str(), openMode);
}

//------------------------------------------------------------------------------
void File::close()
{
	m_fileStream.safeRelease();
}

//------------------------------------------------------------------------------
PathName File::getFilePath() const
{
	return m_filePath.canonicalizePath();
}

//------------------------------------------------------------------------------
String File::getFileName() const
{
	return m_filePath.getFileName();
}

//------------------------------------------------------------------------------
bool File::canRead() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->canRead();
}

//------------------------------------------------------------------------------
bool File::canWrite() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->canWrite();
}

//------------------------------------------------------------------------------
int64_t File::getLength() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->getLength();
}

//------------------------------------------------------------------------------
int64_t File::getPosition() const
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->getPosition();
}

//------------------------------------------------------------------------------
size_t File::read(void* buffer, size_t readCount)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	return m_fileStream->read(buffer, readCount);
}

//------------------------------------------------------------------------------
void File::write(const void* data, size_t byteCount)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->write(data, byteCount);
}

//------------------------------------------------------------------------------
void File::seek(int64_t offset, SeekOrigin origin)
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->seek(offset, origin);
}

//------------------------------------------------------------------------------
void File::flush()
{
	LN_THROW(m_fileStream, InvalidOperationException);
	m_fileStream->flush();
}


//==============================================================================
// File
//==============================================================================

//------------------------------------------------------------------------------
TemporaryFile::TemporaryFile()
	: File(PathName::getUniqueFilePathInDirectory(PathName::getSpecialFolderPath(SpecialFolder::Temporary), nullptr, nullptr))
	, m_autoRemove(true)
{
}

//------------------------------------------------------------------------------
TemporaryFile::~TemporaryFile()
{
	close();

	if (m_autoRemove)
	{
		FileSystem::deleteFile(getFilePath());
	}
}

//------------------------------------------------------------------------------
void TemporaryFile::open()
{
	File::open(FileOpenMode::ReadWrite | FileOpenMode::Truncate);
}

LN_NAMESPACE_END
