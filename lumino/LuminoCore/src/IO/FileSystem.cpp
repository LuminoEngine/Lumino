
#include <sys/types.h>
#include <sys/stat.h>
#include <list>
#include "Internal.hpp"
#include <LuminoCore/Base/Buffer.hpp>
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/IO/Stream.hpp>
#if defined(LN_OS_WIN32)
#include "FileSystem_Win32.hpp"
#else
#include "FileSystem_Unix.hpp"
#endif
#include "PathHelper.hpp"

namespace ln {

//==============================================================================
// PlatformFileFinder

class PlatformFileFinder
    : public RefObject
{
public:
    PlatformFileFinder(const PathChar* dirPath, int dirPathLen, FileAttribute attr, const PathChar* pattern, int patternLen);
    ~PlatformFileFinder();
    bool isWorking() const;
    bool isFirst() const;
    const PathString& getCurrent() const;
    const PathString& getCurrentFileName() const { return m_impl.getCurrentFileName(); }
    Flags<FileAttribute> getFileAttribute() const;
    bool next();

private:
    bool nextInternal(bool* outIsDir);
    void setCurrent(const PathString& nativeCurrent);
    void clearCurrent();

    PlatformFileFinderImpl m_impl;
    PathString m_dirPath;
    PathString m_current;
    Flags<FileAttribute> m_attr;
    PathString m_pattern;
    bool m_first;
};

PlatformFileFinder::PlatformFileFinder(const PathChar* dirPath, int dirPathLen, FileAttribute attr, const PathChar* pattern, int patternLen)
    : m_impl()
    , m_dirPath(dirPath)
    , m_current()
    , m_attr(attr)
    , m_pattern()
    , m_first(true)
{
    if (pattern) {
        m_pattern.assign(pattern, patternLen);
    }

    m_current.reserve(m_dirPath.length() + 32); // min size
    m_impl.initialize(dirPath, dirPathLen);
    next();
}

PlatformFileFinder::~PlatformFileFinder()
{
}

bool PlatformFileFinder::isWorking() const
{
    return m_impl.isWorking();
}

bool PlatformFileFinder::isFirst() const
{
    return m_first;
}

const PathString& PlatformFileFinder::getCurrent() const
{
    return m_current;
}

Flags<FileAttribute> PlatformFileFinder::getFileAttribute() const
{
    if (isWorking()) {
        FileAttribute attr;
        if (PlatformFileSystem::getAttribute(getCurrent().c_str(), &attr)) {
            return attr;
        }
    }
    return FileAttribute::None;
}

bool PlatformFileFinder::next()
{
    bool isDir = false;
    if (m_pattern.empty()) {
        return nextInternal(&isDir);
    } else {
        bool result = false;
        do {
            result = nextInternal(&isDir);

        } while (result && !PlatformFileSystem::matchPath(getCurrent().c_str(), m_pattern.c_str()));

        return result;
    }
}

bool PlatformFileFinder::nextInternal(bool* outIsDir)
{
    bool result = false;
    while (true) {
        result = m_impl.next();
        if (!result) {
            clearCurrent();
            break;
        }
        setCurrent(m_impl.getCurrentFileName());

        FileAttribute attr = FileAttribute::None;
        FileAttribute a;
        if (PlatformFileSystem::getAttribute(getCurrent().c_str(), &a)) {
            attr = a;
        }

        *outIsDir = ((a & FileAttribute::Directory) != FileAttribute::None);

        auto filter = m_attr;
        if ((attr & filter) != FileAttribute::None) {
            break;
        }
    }

    m_first = false;
    return result;
}

void PlatformFileFinder::setCurrent(const PathString& nativeCurrent)
{
    m_current.assign(m_dirPath);
    m_current.append(1, (PathChar)detail::PathTraits::DirectorySeparatorChar);
    m_current.append(m_impl.getCurrentFileName());
}

void PlatformFileFinder::clearCurrent()
{
    m_current.clear();
}

//==============================================================================
// FileSystem

bool FileSystem::existsFile(const StringRef& filePath)
{
    return detail::FileSystemInternal::existsFile(filePath.data(), filePath.length());
}

FileAttribute FileSystem::getAttribute(const StringRef& filePath)
{
    return detail::FileSystemInternal::getAttribute(filePath.data(), filePath.length());
}

void FileSystem::setAttribute(const StringRef& filePath, FileAttribute attr)
{
    detail::FileSystemInternal::setAttribute(filePath.data(), filePath.length(), attr);
}

void FileSystem::copyFile(const StringRef& sourceFileName, const StringRef& destFileName, FileCopyOption option)
{
    detail::FileSystemInternal::copyFile(sourceFileName.data(), sourceFileName.length(), destFileName.data(), destFileName.length(), option);
}

void FileSystem::removeFile(const StringRef& filePath)
{
    detail::FileSystemInternal::removeFile(filePath.data(), filePath.length());
}

bool FileSystem::existsDirectory(const StringRef& path)
{
    Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(path.data(), path.length());
    return attr.hasFlag(FileAttribute::Directory);
}

void FileSystem::createDirectory(const StringRef& path)
{
    detail::FileSystemInternal::createDirectory(path.data(), path.length());
}

void FileSystem::removeDirectory(const StringRef& path, bool recursive)
{
    detail::FileSystemInternal::removeDirectory(path.data(), path.length(), recursive);
}

void FileSystem::copyDirectory(const StringRef& srcPath, const StringRef& dstPath, bool overwrite, bool recursive)
{
    detail::FileSystemInternal::copyDirectory(srcPath.data(), srcPath.length(), dstPath.data(), dstPath.length(), overwrite, recursive);
}

bool FileSystem::matchPath(const StringRef& filePath, const StringRef& pattern)
{
    return detail::FileSystemInternal::matchPath(filePath.data(), filePath.length(), pattern.data(), pattern.length());
}

uint64_t FileSystem::getFileSize(const StringRef& filePath)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath.data(), filePath.length());
    return PlatformFileSystem::getFileSize(localPath.c_str());
}

ByteBuffer FileSystem::readAllBytes(const StringRef& filePath)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath.data(), filePath.length());
    const PathChar mode[] = {'r', 'b', '\0'};
    FILE* fp = PlatformFileSystem::fopen(localPath.c_str(), mode);
    if (LN_ENSURE_IO(fp, ln::format(_TT("Access failed: {0}"), filePath))) return ByteBuffer();

    size_t size = (size_t)detail::FileSystemInternal::getFileSize(fp);
    ByteBuffer buffer(size);
    fread(buffer.data(), 1, size, fp);

    fclose(fp);
    LN_EMSCRIPTEN_LAYZY_FLASH;
    return buffer;
}

static String readAllTextHelper(const ByteBuffer& buffer, TextEncoding* encoding)
{
    if (encoding == nullptr) {
        TextEncoding* e = TextEncoding::getEncoding(EncodingType::UTF8);
        if (buffer.size() >= 3 && memcmp(buffer.data(), e->preamble(), 3) == 0)
            encoding = e;
        else
            encoding = TextEncoding::utf8Encoding();
    }

    return encoding->decode(buffer.data(), buffer.size());
}

String FileSystem::readAllText(const StringRef& filePath, TextEncoding* encoding)
{
    ByteBuffer buffer(FileSystem::readAllBytes(filePath));
    return readAllTextHelper(buffer, encoding);
}

String FileSystem::readAllText(Stream* stream, TextEncoding* encoding)
{
    if (LN_REQUIRE(stream->canRead())) return String::Empty;
    size_t size = stream->length();
    ByteBuffer buffer(size);
    stream->read(buffer.data(), size);
    return readAllTextHelper(buffer, encoding);
}

void FileSystem::writeAllBytes(const StringRef& filePath, const void* buffer, size_t size)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath.data(), filePath.length());
    const PathChar mode[] = {'w', 'b', '\0'};
    FILE* fp = PlatformFileSystem::fopen(localPath.c_str(), mode);
    if (LN_ENSURE(fp != NULL, localPath.c_str())) return;

    fwrite(buffer, 1, size, fp);
    fclose(fp);
    LN_EMSCRIPTEN_LAYZY_FLASH;
}

void FileSystem::writeAllText(const StringRef& filePath, const String& str, TextEncoding* encoding)
{
    encoding = (encoding == nullptr) ? TextEncoding::utf8Encoding().get() : encoding;

    auto buffer = encoding->encode(str);

    writeAllBytes(filePath, buffer.data(), buffer.size());
}

//==============================================================================
// FileSystemInternal

namespace detail {

bool FileSystemInternal::existsFile(const char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    return PlatformFileSystem::existsFile(localPath.c_str());
}
bool FileSystemInternal::existsFile(const wchar_t* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    return PlatformFileSystem::existsFile(localPath.c_str());
}
bool FileSystemInternal::existsFile(const Char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    return PlatformFileSystem::existsFile(localPath.c_str());
}

FileAttribute FileSystemInternal::getAttribute(const char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    FileAttribute attr;
    if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
    return attr;
}
FileAttribute FileSystemInternal::getAttribute(const wchar_t* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    FileAttribute attr;
    if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
    return attr;
}
FileAttribute FileSystemInternal::getAttribute(const Char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    FileAttribute attr;
    if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
    return attr;
}

void FileSystemInternal::setAttribute(const char* filePath, int len, FileAttribute attr)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}
void FileSystemInternal::setAttribute(const wchar_t* filePath, int len, FileAttribute attr)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}
void FileSystemInternal::setAttribute(const Char* filePath, int len, FileAttribute attr)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}

time_t FileSystemInternal::getLastModifiedTime(const char* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
	time_t time;
	PlatformFileSystem::getLastModifiedTime(localPath.c_str(), &time);
	return time;
}
time_t FileSystemInternal::getLastModifiedTime(const wchar_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
	time_t time;
	PlatformFileSystem::getLastModifiedTime(localPath.c_str(), &time);
	return time;
}
time_t FileSystemInternal::getLastModifiedTime(const Char* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
	time_t time;
	PlatformFileSystem::getLastModifiedTime(localPath.c_str(), &time);
	return time;
}

void FileSystemInternal::copyFile(const char* sourceFileName, int sourceFileNameLen, const char* destFileName, int destFileNameLen, FileCopyOption option)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath1(sourceFileName, sourceFileNameLen);
    detail::GenericStaticallyLocalPath<PathChar> localPath2(destFileName, destFileNameLen);
    PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), (option == FileCopyOption::Overwrite));
}
void FileSystemInternal::copyFile(const wchar_t* sourceFileName, int sourceFileNameLen, const wchar_t* destFileName, int destFileNameLen, FileCopyOption option)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath1(sourceFileName, sourceFileNameLen);
    detail::GenericStaticallyLocalPath<PathChar> localPath2(destFileName, destFileNameLen);
    PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), (option == FileCopyOption::Overwrite));
}
void FileSystemInternal::copyFile(const Char* sourceFileName, int sourceFileNameLen, const Char* destFileName, int destFileNameLen, FileCopyOption option)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath1(sourceFileName, sourceFileNameLen);
    detail::GenericStaticallyLocalPath<PathChar> localPath2(destFileName, destFileNameLen);
    PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), (option == FileCopyOption::Overwrite));
}

void FileSystemInternal::removeFile(const char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    if (PlatformFileSystem::existsFile(localPath.c_str())) {
        PlatformFileSystem::removeFile(localPath.c_str());
    }
}
void FileSystemInternal::removeFile(const wchar_t* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    if (PlatformFileSystem::existsFile(localPath.c_str())) {
        PlatformFileSystem::removeFile(localPath.c_str());
    }
}
void FileSystemInternal::removeFile(const Char* filePath, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(filePath, len);
    if (PlatformFileSystem::existsFile(localPath.c_str())) {
        PlatformFileSystem::removeFile(localPath.c_str());
    }
}

bool FileSystemInternal::existsDirectory(const char* filePath, int len)
{
    Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(filePath, len);
    return attr.hasFlag(FileAttribute::Directory);
}
bool FileSystemInternal::existsDirectory(const wchar_t* filePath, int len)
{
    Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(filePath, len);
    return attr.hasFlag(FileAttribute::Directory);
}
bool FileSystemInternal::existsDirectory(const Char* filePath, int len)
{
    Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(filePath, len);
    return attr.hasFlag(FileAttribute::Directory);
}

static void createDirectoryInternal2(const PathChar* begin, const PathChar* end)
{
    if (FileSystemInternal::existsDirectory(begin, (end - begin))) return;

    std::vector<std::basic_string<PathChar>> pathList;
    std::basic_string<PathChar> dir;

    int i = (end - begin) - 1; //StringTraits::tcslen(path) - 1;	// 一番後ろの文字の位置
    while (i >= 0) {
        dir.assign(begin, i + 1);

        FileAttribute attr;
        bool result = PlatformFileSystem::getAttribute(dir.c_str(), &attr);
        if (result && testFlag(attr, FileAttribute::Directory)) {
            break;
        }
        pathList.push_back(dir);

        // セパレータが見つかるまで探す
        while (i > 0 && begin[i] != PathTraits::DirectorySeparatorChar && begin[i] != PathTraits::AltDirectorySeparatorChar) {
            --i;
        }
        --i;
    }

    if (pathList.empty()) {
        return;
    } // path が存在している

    for (int i = pathList.size() - 1; i >= 0; --i) {
        PlatformFileSystem::createDirectory(pathList[i].c_str());
    }
}
void FileSystemInternal::createDirectory(const char* path, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}
void FileSystemInternal::createDirectory(const wchar_t* path, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}
void FileSystemInternal::createDirectory(const Char* path, int len)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}

static void deleteDirectoryInternal(const PathChar* path, int len, bool recursive)
{
    if (recursive) {
        PlatformFileFinder finder(path, len, FileAttribute::All, nullptr, 0);
        while (finder.isWorking()) {
            auto& current = finder.getCurrent();
            if (finder.getFileAttribute().hasFlag(FileAttribute::Directory)) {
                deleteDirectoryInternal(current.c_str(), current.length(), recursive); // recursive
            } else                                                                     // TODO: 他の属性みないとダメ。シンボリックリンクとか
            {
                PlatformFileSystem::removeFile(current.c_str());
            }
            finder.next();
        }
    }
    PlatformFileSystem::removeDirectory(path);
}
void FileSystemInternal::removeDirectory(const char* path, int len, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}
void FileSystemInternal::removeDirectory(const wchar_t* path, int len, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}
void FileSystemInternal::removeDirectory(const Char* path, int len, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, len);
    deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}

static void copyDirectoryInternal(
    const PathChar* srcPath,
    const PathChar* dstPath,
    bool overwrite,
    bool recursive)
{
    // 上書きしないとき、すでにフォルダが存在してはならない
    if (!overwrite) {
        FileAttribute attr;
        if (PlatformFileSystem::getAttribute(srcPath, &attr) &&
            testFlag(attr, FileAttribute::Directory)) {
            LN_ENSURE_IO(0);
            return;
        }
    }

    // コピー先フォルダを作っておく
    PlatformFileSystem::createDirectory(dstPath);

    PlatformFileFinder finder(srcPath, StringHelper::strlen(srcPath), FileAttribute::All, nullptr, 0);
    PathString dest;
    while (finder.isWorking()) {
        const PathString& src = finder.getCurrent();
        dest.assign(dstPath);
        dest.append(1, PathTraits::DirectorySeparatorChar);
        dest.append(finder.getCurrentFileName());

        FileAttribute srcAttr;
        if (!PlatformFileSystem::getAttribute(src.c_str(), &srcAttr)) srcAttr = FileAttribute::None;

        FileAttribute dstAttr;
        if (!PlatformFileSystem::getAttribute(dest.c_str(), &dstAttr)) dstAttr = FileAttribute::None;

        if (testFlag(srcAttr, FileAttribute::Normal)) {
            if (testFlag(dstAttr, FileAttribute::Directory)) {
                // TODO: src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
                LN_NOTIMPLEMENTED();
            } else if (testFlag(dstAttr, FileAttribute::Normal)) {
                // コピー先にファイルとして存在していて、上書きする場合はコピーする
                if (overwrite) {
                    PlatformFileSystem::copyFile(src.c_str(), dest.c_str(), true);
                }
            } else {
                PlatformFileSystem::copyFile(src.c_str(), dest.c_str(), false);
            }
        } else if (testFlag(srcAttr, FileAttribute::Directory)) {
            if (testFlag(dstAttr, FileAttribute::Normal)) {
                // TODO: src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
                LN_NOTIMPLEMENTED();
            }

            if (recursive) {
                copyDirectoryInternal(src.c_str(), dest.c_str(), overwrite, recursive);
            }
        }

        finder.next();
    }
}
void FileSystemInternal::copyDirectory(const char* srcPath, int srcPathLen, const char* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localSrcPath(srcPath, srcPathLen);
    detail::GenericStaticallyLocalPath<PathChar> localDstPath(dstPath, dstPathLen);
    copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
void FileSystemInternal::copyDirectory(const wchar_t* srcPath, int srcPathLen, const wchar_t* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localSrcPath(srcPath, srcPathLen);
    detail::GenericStaticallyLocalPath<PathChar> localDstPath(dstPath, dstPathLen);
    copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
void FileSystemInternal::copyDirectory(const Char* srcPath, int srcPathLen, const Char* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
    detail::GenericStaticallyLocalPath<PathChar> localSrcPath(srcPath, srcPathLen);
    detail::GenericStaticallyLocalPath<PathChar> localDstPath(dstPath, dstPathLen);
    copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
bool FileSystemInternal::matchPath(const char* path, int pathLen, const char* pattern, int patternLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localPattern(pattern, patternLen);
    return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}
bool FileSystemInternal::matchPath(const wchar_t* path, int pathLen, const wchar_t* pattern, int patternLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localPattern(pattern, patternLen);
    return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}
bool FileSystemInternal::matchPath(const Char* path, int pathLen, const Char* pattern, int patternLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localPattern(pattern, patternLen);
    return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}

FILE* FileSystemInternal::fopen(const char* path, int pathLen, const char* mode, int modeLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localMode(mode, modeLen);
    return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}
FILE* FileSystemInternal::fopen(const wchar_t* path, int pathLen, const wchar_t* mode, int modeLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localMode(mode, modeLen);
    return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}
FILE* FileSystemInternal::fopen(const Char* path, int pathLen, const Char* mode, int modeLen)
{
    detail::GenericStaticallyLocalPath<PathChar> localPath(path, pathLen);
    detail::GenericStaticallyLocalPath<PathChar> localMode(mode, modeLen);
    return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}

uint64_t FileSystemInternal::getFileSize(FILE* stream)
{
    return PlatformFileSystem::getFileSize(stream);
}

// 現在の位置とデータ(ファイル)サイズ、オフセット、基準(SEEK_xxxx)を受け取って、新しいシーク位置を返す
int64_t FileSystemInternal::calcSeekPoint(int64_t curPoint, int64_t maxSize, int64_t offset, int origin)
{
    int64_t newPoint = curPoint;
    switch (origin) {
        case SEEK_CUR:
            newPoint += offset;
            break;

        case SEEK_END:
            newPoint = maxSize + offset;
            break;

        default:
            newPoint = offset;
            break;
    }

    if (newPoint < 0) {
        newPoint = 0;
    }
    if (newPoint > maxSize) {
        newPoint = maxSize;
    }
    return newPoint;
}

void FileGlob::glob(const Path& rootDir, const Path& pathspec, const List<String>& filters, bool recursive, List<Path>* outPathes)
{
    auto path = ln::Path(rootDir, pathspec);

    if (ln::FileSystem::existsFile(path))
    {
        // ファイルを指していたらそのまま追加
        outPathes->add(path);
    }
    else if (ln::FileSystem::existsDirectory(path))
    {
        // フォルダだったらその中のファイルを全部追加
        auto files = ln::FileSystem::getFiles(path, ln::StringRef(), (recursive) ? ln::SearchOption::Recursive : ln::SearchOption::TopDirectoryOnly);
        for (auto& file : files) {
            if (filterFilePath(filters, file)) {
                outPathes->add(file);
            }
        }
    }
    else if (pathspec.str().contains('*'))
    {
        // ワイルドカード入っているっぽい
        auto files = ln::FileSystem::getFiles(path.parent(), pathspec.fileName(), (recursive) ? ln::SearchOption::Recursive : ln::SearchOption::TopDirectoryOnly);
        for (auto& file : files) {
            if (filterFilePath(filters, file)) {
                outPathes->add(file);
            }
        }
    }
    else
    {
        LN_ERROR("invalid path");
    }
}

bool FileGlob::filterFilePath(const ln::List<ln::String>& filters, const ln::String& filePath)
{
    return std::any_of(filters.begin(), filters.end(), [&filePath](const ln::String& filter)
    {
        return ln::StringHelper::match(filter.c_str(), filePath.c_str());
    });
}

} // namespace detail

namespace detail {

class DirectoryIterator2Impl
    : public RefObject
{
public:
    DirectoryIterator2Impl(const StringRef& path, const StringRef& searchPattern, SearchOption searchOption, SearchTargetEntity targetEntity)
        : m_path(path)
        , m_searchPattern(searchPattern.data(), searchPattern.length()) // TODO: check
        //, m_filterAttr(FileAttribute::All)
        , m_searchTargetEntity(targetEntity) // TODO:
        , m_searchOption(searchOption)
    {
    }

    ~DirectoryIterator2Impl()
    {
    }

    const Path& currentPath() const { return m_currentPath; }

    // 頭出し
    void setup()
    {
        pushFilder();
        nextInternal2(true);
    }

    bool next()
    {
        return nextInternal2(false);
    }

private:
    bool nextInternal2(bool setup)
    {
        bool first = true;
        bool result;
        do {
            if (setup && first) {
                result = current()->isWorking();
            } else {
                result = nextInternal(setup);
            }

            setup = first = false;

            if (result) {
                if (m_searchTargetEntity == SearchTargetEntity::Directory &&
                    current()->getFileAttribute().hasFlag(FileAttribute::Directory)) {
                    if (m_searchPattern.isEmpty()) {
                        break; // パターン指定なし。返す。
                    }

                    const auto& path = current()->getCurrent();
                    const auto* name = PathTraits::getFileName(path.c_str(), path.c_str() + path.length());
                    if (PlatformFileSystem::matchPath(name, m_searchPattern.c_str())) {
                        break; // パターンに一致。返す。
                    }
                }
                if (m_searchTargetEntity == SearchTargetEntity::File &&
                    !current()->getFileAttribute().hasFlag(FileAttribute::Directory)) {
                    if (m_searchPattern.isEmpty()) {
                        break; // パターン指定なし。返す。
                    }
                    
                    const auto& path = current()->getCurrent();
                    const auto* name = PathTraits::getFileName(path.c_str(), path.c_str() + path.length());
                    if (PlatformFileSystem::matchPath(name, m_searchPattern.c_str())) {
                        break; // パターンに一致。返す。
                    }
                }
            }

        } while (result /* && !m_filterAttr.TestFlag((FileAttribute::enum_type)current()->getFileAttribute().getValue())*/);

        if (result) {
            m_currentPath = String::fromStdString(current()->getCurrent());
        } else {
            m_currentPath.clear();
        }

        return result;
    }

    bool nextInternal(bool setup)
    {
        if (m_searchOption == SearchOption::Recursive &&
            current()->getFileAttribute().hasFlag(FileAttribute::Directory)) {
            pushFilder();
        } else {
            if (!setup)
                current()->next();
        }

        while (!current()->isWorking()) {
            popFinder(); // 戻す
            if (m_stack.empty()) return false;

            current()->next(); // 戻したフォルダを見ているので1つ進める
        }
        return current()->isWorking();
    }

    // 関数を抜けたとき、stack top は既に次に返すべき Entity を指している
    // または、Entity が一つもなければ isWorkding が false になっている
    void pushFilder()
    {
        if (m_stack.empty()) {
            detail::GenericStaticallyLocalPath<PathChar> localPath(m_path.c_str(), m_path.length());
            m_stack.push_back(makeRef<PlatformFileFinder>(localPath.c_str(), localPath.getLength(), FileAttribute::All, nullptr, 0 /*, m_filterAttr | FileAttribute::Directory, m_searchPattern.c_str(), m_searchPattern.length()*/));
        } else {
            m_stack.push_back(makeRef<PlatformFileFinder>(m_stack.back()->getCurrent().c_str(), m_stack.back()->getCurrent().length(), FileAttribute::All, nullptr, 0 /*, m_filterAttr | FileAttribute::Directory, m_searchPattern.c_str(), m_searchPattern.length()*/));
        }
    }

    void popFinder()
    {
        m_stack.pop_back();
    }

    PlatformFileFinder* current() const
    {
        return m_stack.back();
    }

    Path m_path;
    detail::GenericStaticallyLocalPath<PathChar> m_searchPattern;
    SearchTargetEntity m_searchTargetEntity;
    SearchOption m_searchOption;
    std::list<Ref<PlatformFileFinder>> m_stack;
    Path m_currentPath;
};

DirectoryIterator2::DirectoryIterator2()
    : m_impl()
{
}

DirectoryIterator2::DirectoryIterator2(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption, SearchTargetEntity targetEntity)
    : m_impl()
{
    m_impl = makeRef<DirectoryIterator2Impl>(dirPath, pattern, searchOption, targetEntity);
    m_impl->setup();
    m_current = m_impl->currentPath();
}

DirectoryIterator2::DirectoryIterator2(const DirectoryIterator2& other)
    : m_impl(other.m_impl)
    , m_current(other.m_current)
{
}

DirectoryIterator2::~DirectoryIterator2()
{
}

DirectoryIterator2& DirectoryIterator2::operator=(const DirectoryIterator2& other)
{
    m_impl = other.m_impl;
    m_current = other.m_current;
    return *this;
}

DirectoryIterator2& DirectoryIterator2::operator++() // prefix
{
    if (m_impl != nullptr) {
        m_impl->next();
        m_current = m_impl->currentPath();
    }
    return *this;
}

DirectoryIterator2 DirectoryIterator2::operator++(int) // postfix
{
    if (m_impl != nullptr) {
        m_impl->next();
        m_current = m_impl->currentPath();
    }
    return *this;
}

} // namespace detail

DirectoryIteratorRange::DirectoryIteratorRange(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption, detail::SearchTargetEntity targetEntity)
    : m_begin(dirPath, pattern, searchOption, targetEntity)
    , m_end()
{
}

Path FileSystem::getFile(const StringRef& dirPath, const StringRef& pattern)
{
	auto files = ln::FileSystem::getFiles(dirPath, pattern);
	if (!files.isEmpty()) {
		return *files.begin();
	}
	else {
		return Path();
	}
}

DirectoryIteratorRange FileSystem::getFiles(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption)
{
    return DirectoryIteratorRange(dirPath, pattern, searchOption, detail::SearchTargetEntity::File);
}

DirectoryIteratorRange FileSystem::getDirectories(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption)
{
    return DirectoryIteratorRange(dirPath, pattern, searchOption, detail::SearchTargetEntity::Directory);
}

} // namespace ln
