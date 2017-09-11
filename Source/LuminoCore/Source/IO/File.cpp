
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
	if (LN_REQUIRE(!m_fileStream)) return;	// すでにファイルが開かれている
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
	if (LN_REQUIRE(m_fileStream)) return false;
	return m_fileStream->canRead();
}

//------------------------------------------------------------------------------
bool File::canWrite() const
{
	if (LN_REQUIRE(m_fileStream)) return false;
	return m_fileStream->canWrite();
}

//------------------------------------------------------------------------------
int64_t File::getLength() const
{
	if (LN_REQUIRE(m_fileStream)) return 0;
	return m_fileStream->getLength();
}

//------------------------------------------------------------------------------
int64_t File::getPosition() const
{
	if (LN_REQUIRE(m_fileStream)) return 0;
	return m_fileStream->getPosition();
}

//------------------------------------------------------------------------------
size_t File::read(void* buffer, size_t readCount)
{
	if (LN_REQUIRE(m_fileStream)) return 0;
	return m_fileStream->read(buffer, readCount);
}

//------------------------------------------------------------------------------
void File::write(const void* data, size_t byteCount)
{
	if (LN_REQUIRE(m_fileStream)) return;
	m_fileStream->write(data, byteCount);
}

//------------------------------------------------------------------------------
void File::seek(int64_t offset, SeekOrigin origin)
{
	if (LN_REQUIRE(m_fileStream)) return;
	m_fileStream->seek(offset, origin);
}

//------------------------------------------------------------------------------
void File::flush()
{
	if (LN_REQUIRE(m_fileStream)) return;
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
