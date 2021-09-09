/*
    # Note: std::fstream は使わないの？
    
    Windows 環境で問題がある。
    
    std::fstream はファイルパスを char* で指定する必要があるが、
    エンコーディングは OS のロケールに依存する。日本語であれば SJIS。
    Lumino の内部エンコーディングは Unicode。SJIS に変換するときに情報が失われることがある。
    
    std::wfstream はファイルパス wchar_t* なので Unicode 情報が失われることはないが、
    ファイル内容が常に UTF16 として出てきてしまう。（たとえ utf-8 のソースコードをバイナリモードで開いても）
*/

#include "Internal.hpp"
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/IO/FileStream.hpp>

namespace ln {

//==============================================================================
// FileStream

Ref<FileStream> FileStream::create(const StringRef& filePath, FileOpenMode openMode)
{
    Ref<FileStream> ptr(LN_NEW FileStream(), false);
    ptr->open(filePath, openMode);
    return ptr;
}

FileStream::FileStream()
    : m_stream(nullptr)
    , m_openModeFlags(FileOpenMode::None)
    , m_writeLen(0)
{
    detail::RefObjectInternal::setValidObject(this, false);
}

FileStream::~FileStream()
{
    close();
}

bool FileStream::open(const StringRef& filePath, FileOpenMode openMode)
{
    close();

    m_filePath = Path(filePath); //Path(filePath).canonicalize();
    m_openModeFlags = openMode;

    if (m_openModeFlags.hasFlag(FileOpenMode::Deferring)) {
        detail::GenericStaticallyLocalPath<PathChar> localPath(m_filePath.c_str(), m_filePath.length());
        if (!detail::FileSystemInternal::existsFile(localPath.c_str(), localPath.getLength())) {
            LN_ENSURE_IO(0, filePath);
            return false;
        }
    }
    // 遅延オープンでなければここで開いてしまう
    else {
        open();

        if (m_openModeFlags.hasFlag(FileOpenMode::Append)) {
            m_writeLen = detail::FileSystemInternal::getFileSize(m_stream);
        }
    }

    if (m_stream) {
		detail::RefObjectInternal::setValidObject(this, true);
    }
    return true;
}

void FileStream::close()
{
    if (m_stream != nullptr) {
        fclose(m_stream);
        m_stream = nullptr;
    }
}

bool FileStream::canRead() const
{
    return (m_openModeFlags.hasFlag(FileOpenMode::Read));
}

bool FileStream::canWrite() const
{
    return (m_openModeFlags.hasFlag(FileOpenMode::Write));
}

int64_t FileStream::length() const
{
    checkOpen();
    int64_t rawLen = detail::FileSystemInternal::getFileSize(m_stream);
    return (rawLen < m_writeLen) ? m_writeLen : rawLen;
}

int64_t FileStream::position() const
{
    checkOpen();
    // TODO: 64bit 確認 → ftello?
    return ftell(m_stream);
}

size_t FileStream::read(void* buffer, size_t readCount)
{
    checkOpen();
    return fread(buffer, 1, readCount, m_stream);
}

void FileStream::write(const void* data, size_t byteCount)
{
    checkOpen();
    size_t size = fwrite(data, 1, byteCount, m_stream);
    LN_ENSURE(size == byteCount);
    m_writeLen += size;
}

void FileStream::seek(int64_t offset, SeekOrigin origin)
{
    checkOpen();

#ifdef LN_OS_WIN32
    int r = _fseeki64(m_stream, offset, static_cast<int>(origin));
#else
    // TODO:
    // http://stackoverflow.com/questions/1035657/seeking-and-reading-large-files-in-a-linux-c-application
    fseek(m_stream, offset, static_cast<int>(origin));
#endif
}

void FileStream::flush()
{
    checkOpen();
    fflush(m_stream);
}

void FileStream::checkOpen() const
{
    if (m_openModeFlags.hasFlag(FileOpenMode::Deferring)) {
        if (m_stream == NULL) {
            open();
        }
    } else {
        LN_ENSURE(m_stream != NULL);
    }
}

void FileStream::open() const
{
    if (LN_REQUIRE(m_stream == NULL)) return;

    const CChar* mode = NULL;
    if (m_openModeFlags.hasFlag(FileOpenMode::ReadWrite)) {
        if (m_openModeFlags.hasFlag(FileOpenMode::Append)) {
            mode = _CT("a+b"); // 読み取りと書き込み (末尾に追加する)
        } else if (m_openModeFlags.hasFlag(FileOpenMode::Truncate)) {
            mode = _CT("w+b"); // 読み取りと書き込み (ファイルを空にする)
        } else {
            mode = _CT("r+b"); // 読み取りと書き込み (ファイルが存在しない場合はエラー)
        }
    } else if (m_openModeFlags.hasFlag(FileOpenMode::Write)) {
        if (m_openModeFlags.hasFlag(FileOpenMode::Append)) {
            mode = _CT("ab"); // 書き込み (末尾に追加する。ファイルが無ければ新規作成)
        } else if (m_openModeFlags.hasFlag(FileOpenMode::Truncate)) {
            mode = _CT("wb"); // 書き込み (ファイルを空にする)
        } else {
            mode = _CT("wb"); // 書き込み (モード省略。Truncate)
        }
    } else if (m_openModeFlags.hasFlag(FileOpenMode::Read)) {
        if (m_openModeFlags.hasFlag(FileOpenMode::Append)) {
            mode = NULL; // 読み込みなのに末尾追加はできない
        } else if (m_openModeFlags.hasFlag(FileOpenMode::Truncate)) {
            mode = NULL; // 読み込みなのにファイルを空にはできない
        } else {
            mode = _CT("rb"); // 読み込み
        }
    }
    if (LN_REQUIRE(mode)) return;

    detail::GenericStaticallyLocalPath<PathChar> localPath(m_filePath.c_str(), m_filePath.length());
    m_stream = detail::FileSystemInternal::fopen(localPath.c_str(), localPath.getLength(), mode, StringHelper::strlen(mode));
    LN_ENSURE_IO(m_stream != nullptr, _TT("{}"), m_filePath.c_str());
}

} // namespace ln
