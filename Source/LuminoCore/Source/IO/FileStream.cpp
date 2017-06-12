
#include "../Internal.h"
#include "../../include/Lumino/IO/FileSystem.h"
#include "../../include/Lumino/IO/FileStream.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GenericFileStream
//==============================================================================

//------------------------------------------------------------------------------
template<typename TChar>
RefPtr<GenericFileStream<TChar>> GenericFileStream<TChar>::create(const TChar* filePath, FileOpenMode openMode)
{
	RefPtr<GenericFileStream<TChar>> ptr(LN_NEW GenericFileStream(), false);
	ptr->open(filePath, openMode);
	return ptr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericFileStream<TChar>::GenericFileStream()
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
template<typename TChar>
GenericFileStream<TChar>::~GenericFileStream()
{
	close();
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::open(const TChar* filePath, FileOpenMode openMode)
{
	LN_ASSERT( filePath );
	close();

	m_filePath = PathName(filePath).canonicalizePath();
	m_openModeFlags = openMode;

	if (m_openModeFlags.TestFlag(FileOpenMode::Deferring))
	{
		if (!FileSystem::existsFile(filePath)) {
			LN_THROW(0, FileNotFoundException, filePath);
		}
	}
	// 遅延オープンでなければここで開いてしまう
	else {
		open();
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::close()
{
	if (m_stream != NULL) {
		fclose(m_stream);
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericFileStream<TChar>::canRead() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::read));
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericFileStream<TChar>::canWrite() const
{
	return (m_openModeFlags.TestFlag(FileOpenMode::write));
}

//------------------------------------------------------------------------------
template<typename TChar>
int64_t GenericFileStream<TChar>::getLength() const
{
	checkOpen();
	return (size_t)FileSystem::getFileSize(m_stream);
}

//------------------------------------------------------------------------------
template<typename TChar>
int64_t GenericFileStream<TChar>::getPosition() const
{
	checkOpen();
	// TODO: 64bit 確認 → ftello?
	return ftell(m_stream);
}

//------------------------------------------------------------------------------
template<typename TChar>
size_t GenericFileStream<TChar>::read(void* buffer, size_t readCount)
{
	checkOpen();
	return fread(buffer, 1, readCount, m_stream);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::write(const void* data, size_t byteCount)
{
	checkOpen();
	size_t nWriteSize = fwrite( data, 1, byteCount, m_stream );
	LN_THROW(nWriteSize == byteCount, InvalidOperationException);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::seek(int64_t offset, SeekOrigin origin)
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
template<typename TChar>
void GenericFileStream<TChar>::flush()
{
	checkOpen();
	fflush(m_stream);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::checkOpen() const
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
		LN_THROW(m_stream != NULL, InvalidOperationException);
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericFileStream<TChar>::open() const
{
	LN_THROW(m_stream == NULL, InvalidOperationException);

	const TCHAR* mode = NULL;
	if (m_openModeFlags.TestFlag(FileOpenMode::ReadWrite))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::append)) {
			mode = _T("a+b");		// 読み取りと書き込み (末尾に追加する)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _T("w+b");		// 読み取りと書き込み (ファイルを空にする)
		}
		else {
			mode = _T("r+b");		// 読み取りと書き込み (ファイルが存在しない場合はエラー)
		}
	}
	else if (m_openModeFlags.TestFlag(FileOpenMode::write))
	{
		if (m_openModeFlags.TestFlag(FileOpenMode::append)) {
			mode = _T("ab");		// 書き込み (末尾に追加する。ファイルが無ければ新規作成)
		}
		else if (m_openModeFlags.TestFlag(FileOpenMode::Truncate)) {
			mode = _T("wb");		// 書き込み (ファイルを空にする)
		}
		else {
			mode = _T("wb");		// 書き込み (モード省略。Truncate)
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
			mode = _T("rb");		// 読み込み
		}
	}
	LN_THROW(mode, ArgumentException);

	auto path = String(m_filePath.getString());
	errno_t err = _tfopen_s(&m_stream, path.c_str(), mode);
	LN_THROW(err == 0, FileNotFoundException);
}

// Instantiate template
template class GenericFileStream<char>;
template class GenericFileStream<wchar_t>;

LN_NAMESPACE_END
