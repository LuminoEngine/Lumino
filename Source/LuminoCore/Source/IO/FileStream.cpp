
#include "../Internal.h"
#include "../../Include/Lumino/IO/FileSystem.h"
#include "../../Include/Lumino/IO/FileStream.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GenericFileStream
//==============================================================================

//------------------------------------------------------------------------------
Ref<FileStream> FileStream::create(const Char* filePath, FileOpenMode openMode)
{
	Ref<FileStream> ptr(LN_NEW FileStream(), false);
	ptr->open(filePath, openMode);
	return ptr;
}

//------------------------------------------------------------------------------
FileStream::FileStream()
	: m_stream(nullptr)
	, m_openModeFlags(FileOpenMode::None)
{
}

//------------------------------------------------------------------------------
//GenericFileStream::GenericFileStream(const TChar* filePath, FileOpenMode openMode)
//	: m_stream(NULL)
//	, m_openModeFlags(FileOpenMode::None)
//{
//	Open(filePath, openMode);
//}

//------------------------------------------------------------------------------
FileStream::~FileStream()
{
	close();
}

//------------------------------------------------------------------------------
void FileStream::open(const Char* filePath, FileOpenMode openMode)
{
	LN_ASSERT( filePath );
	close();

	m_filePath = PathName(filePath).canonicalizePath();
	m_openModeFlags = openMode;

	if (m_openModeFlags.TestFlag(FileOpenMode::Deferring))
	{
		if (!detail::FileSystemInternal::existsFile(filePath, StringTraits::tcslen(filePath)))
		{
			LN_ENSURE_FILE_NOT_FOUND(0, filePath);
			return;
		}
	}
	// 遅延オープンでなければここで開いてしまう
	else
	{
		open();
	}
}

//------------------------------------------------------------------------------
void FileStream::close()
{
	if (m_stream != NULL) {
		fclose(m_stream);
	}
}

//------------------------------------------------------------------------------
bool FileStream::canRead() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::read));
}

//------------------------------------------------------------------------------
bool FileStream::canWrite() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::write));
}

//------------------------------------------------------------------------------
int64_t FileStream::getLength() const
{
	checkOpen();
	return (size_t)FileSystem::getFileSize(m_stream);
}

//------------------------------------------------------------------------------
int64_t FileStream::getPosition() const
{
	checkOpen();
	// TODO: 64bit 確認 → ftello?
	return ftell(m_stream);
}

//------------------------------------------------------------------------------
size_t FileStream::read(void* buffer, size_t readCount)
{
	checkOpen();
	return fread(buffer, 1, readCount, m_stream);
}

//------------------------------------------------------------------------------
void FileStream::write(const void* data, size_t byteCount)
{
	checkOpen();
	size_t nWriteSize = fwrite( data, 1, byteCount, m_stream );
	LN_ENSURE(nWriteSize == byteCount);
}

//------------------------------------------------------------------------------
void FileStream::seek(int64_t offset, SeekOrigin origin)
{
	checkOpen();

#ifdef LN_OS_WIN32
	_fseeki64(m_stream, offset, origin);
#else
	// TODO:
	// http://stackoverflow.com/questions/1035657/seeking-and-reading-large-files-in-a-linux-c-application
	fseek(m_stream, offset, origin);
#endif
}

//------------------------------------------------------------------------------
void FileStream::flush()
{
	checkOpen();
	fflush(m_stream);
}

//------------------------------------------------------------------------------
void FileStream::checkOpen() const
{
	if (m_openModeFlags.TestFlag(FileOpenMode::Deferring))
	{
		if (m_stream == NULL)
		{
			open();
		}
	}
	else
	{
		LN_ENSURE(m_stream != NULL);
	}
}

//------------------------------------------------------------------------------
void FileStream::open() const
{
	if (LN_REQUIRE(m_stream == NULL)) return;

	const Char* mode = NULL;
	if (m_openModeFlags.TestFlag(FileOpenMode::ReadWrite))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::append)) {
			mode = _TT("a+b");		// 読み取りと書き込み (末尾に追加する)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _TT("w+b");		// 読み取りと書き込み (ファイルを空にする)
		}
		else {
			mode = _TT("r+b");		// 読み取りと書き込み (ファイルが存在しない場合はエラー)
		}
	}
	else if (m_openModeFlags.TestFlag(FileOpenMode::write))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::append)) {
			mode = _TT("ab");		// 書き込み (末尾に追加する。ファイルが無ければ新規作成)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _TT("wb");		// 書き込み (ファイルを空にする)
		}
		else {
			mode = _TT("wb");		// 書き込み (モード省略。Truncate)
		}
	}
	else if (m_openModeFlags.TestFlag(FileOpenMode::read))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::append)) {
			mode = NULL;			// 読み込みなのに末尾追加はできない
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = NULL;			// 読み込みなのにファイルを空にはできない
		}
		else {
			mode = _TT("rb");		// 読み込み
		}
	}
	if (LN_REQUIRE(mode)) return;

	m_stream = detail::FileSystemInternal::fopen(m_filePath.c_str(), m_filePath.getLength(), mode, StringTraits::tcslen(mode));
	LN_ENSURE_FILE_NOT_FOUND(m_stream != nullptr, m_filePath.c_str());
}

LN_NAMESPACE_END
