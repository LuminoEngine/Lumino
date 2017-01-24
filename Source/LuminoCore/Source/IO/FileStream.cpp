
#include "../Internal.h"
#include "../../include/Lumino/IO/FileSystem.h"
#include "../../include/Lumino/IO/FileStream.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// FileStream
//==============================================================================

//------------------------------------------------------------------------------
FileStreamPtr FileStream::Create(const TCHAR* filePath, FileOpenMode openMode)
{
	FileStreamPtr ptr(LN_NEW FileStream(), false);
	ptr->Open(filePath, openMode);
	return ptr;
}

//------------------------------------------------------------------------------
FileStream::FileStream()
	: m_stream(nullptr)
	, m_openModeFlags(FileOpenMode::None)
{
}

//------------------------------------------------------------------------------
//FileStream::FileStream(const TCHAR* filePath, FileOpenMode openMode)
//	: m_stream(NULL)
//	, m_openModeFlags(FileOpenMode::None)
//{
//	Open(filePath, openMode);
//}

//------------------------------------------------------------------------------
FileStream::~FileStream()
{
	Close();
}

//------------------------------------------------------------------------------
void FileStream::Open(const TCHAR* filePath, FileOpenMode openMode)
{
	LN_ASSERT( filePath );
	Close();

	m_filePath = PathName(filePath).CanonicalizePath();
	m_openModeFlags = openMode;

	if (m_openModeFlags.TestFlag(FileOpenMode::Deferring))
	{
		if (!FileSystem::ExistsFile(filePath)) {
			LN_THROW(0, FileNotFoundException, filePath);
		}
	}
	// 遅延オープンでなければここで開いてしまう
	else {
		Open();
	}
}

//------------------------------------------------------------------------------
void FileStream::Close()
{
	if (m_stream != NULL) {
		fclose(m_stream);
	}
}

//------------------------------------------------------------------------------
bool FileStream::CanRead() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::Read));
}

//------------------------------------------------------------------------------
bool FileStream::CanWrite() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::Write));
}

//------------------------------------------------------------------------------
int64_t FileStream::GetLength() const
{
	CheckOpen();
	return (size_t)FileSystem::GetFileSize(m_stream);
}

//------------------------------------------------------------------------------
int64_t FileStream::GetPosition() const
{
	CheckOpen();
	// TODO: 64bit 確認 → ftello?
	return ftell(m_stream);
}

//------------------------------------------------------------------------------
size_t FileStream::Read(void* buffer, size_t readCount)
{
	CheckOpen();
	return fread(buffer, 1, readCount, m_stream);
}

//------------------------------------------------------------------------------
void FileStream::Write( const void* data, size_t byteCount )
{
	CheckOpen();
	size_t nWriteSize = fwrite( data, 1, byteCount, m_stream );
	LN_THROW(nWriteSize == byteCount, InvalidOperationException);
}

//------------------------------------------------------------------------------
void FileStream::Seek(int64_t offset, SeekOrigin origin)
{
	CheckOpen();

#ifdef LN_OS_WIN32
	_fseeki64(m_stream, offset, origin);
#else
	// TODO:
	// http://stackoverflow.com/questions/1035657/seeking-and-reading-large-files-in-a-linux-c-application
	fseek(m_stream, offset, origin);
#endif
}

//------------------------------------------------------------------------------
void FileStream::Flush()
{
	CheckOpen();
	fflush(m_stream);
}

//------------------------------------------------------------------------------
void FileStream::CheckOpen() const
{
	if (m_openModeFlags.TestFlag(FileOpenMode::Deferring))
	{
		if (m_stream == NULL)
		{
			Open();
		}
	}
	else
	{
		LN_THROW(m_stream != NULL, InvalidOperationException);
	}
}

//------------------------------------------------------------------------------
void FileStream::Open() const
{
	LN_THROW(m_stream == NULL, InvalidOperationException);

	const TCHAR* mode = NULL;
	if (m_openModeFlags.TestFlag(FileOpenMode::ReadWrite))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::Append)) {
			mode = _T("a+b");		// 読み取りと書き込み (末尾に追加する)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _T("w+b");		// 読み取りと書き込み (ファイルを空にする)
		}
		else {
			mode = _T("r+b");		// 読み取りと書き込み (ファイルが存在しない場合はエラー)
		}
	}
	else if (m_openModeFlags.TestFlag(FileOpenMode::Write))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::Append)) {
			mode = _T("ab");		// 書き込み (末尾に追加する。ファイルが無ければ新規作成)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _T("wb");		// 書き込み (ファイルを空にする)
		}
		else {
			mode = _T("wb");		// 書き込み (モード省略。Truncate)
		}
	}
	else if (m_openModeFlags.TestFlag(FileOpenMode::Read))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::Append)) {
			mode = NULL;			// 読み込みなのに末尾追加はできない
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = NULL;			// 読み込みなのにファイルを空にはできない
		}
		else {
			mode = _T("rb");		// 読み込み
		}
	}
	LN_THROW(mode, ArgumentException);

	errno_t err = _tfopen_s(&m_stream, m_filePath, mode);
	LN_THROW(err == 0, FileNotFoundException);
}

LN_NAMESPACE_END
