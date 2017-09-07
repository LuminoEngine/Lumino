
#include <sys/types.h>
#include <sys/stat.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringU.h>
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#if defined(LN_OS_WIN32)
#include "FileSystem_Win32.h"
#else
#include "FileSystem_Unix.h"
#endif

LN_NAMESPACE_BEGIN
#if 0


#ifdef _WIN32
#else
//------------------------------------------------------------------------------
static bool is_stat_writable(struct stat *st, const char *path)
{
	// 制限なしに書き込み可であるか
	if (st->st_mode & S_IWOTH)
		return 1;
	// 現在のユーザーIDに許可されているか
	if ((st->st_uid == geteuid()) && (st->st_mode & S_IWUSR))
		return 1;
	// 現在のグループIDに許可されているか
	if ((st->st_gid == getegid()) && (st->st_mode & S_IWGRP))
		return 1;
	// もうここに来ることはほとんどないはずだが念のため
	return access(path, W_OK) == 0;
}
#endif

//------------------------------------------------------------------------------
bool FileSystem::Exists(const char* filePath)
{
	// ※fopen によるチェックはNG。ファイルが排他ロックで開かれていた時に失敗する。
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesA(filePath);
	return ((attr != -1) &&
			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
	// http://www.ie.u-ryukyu.ac.jp/~kono/lecture/1999/os/info1/file-2.html
	//struct stat st;
	//int ret = ::stat(file, &st);
	//st.st_mode
	int ret = access(path, F_OK);
	if (ret == -1) {
		if (errno == ENOENT) {
			return false;
		}
		else {
			// パスが長い、メモリが足りない等理由は様々。
			// http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/faccessat.2.html
			LN_THROW(0, IOException, strerror(errno));
		}
	}
	return true;
#endif
}
bool FileSystem::Exists(const wchar_t* filePath)
{
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesW(filePath);
	return ((attr != -1) &&
			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
	LN_LOCAL_MBCS_FILEPATH(mbcsFilePath, filePath);
	return Exists(mbcsFilePath);
#endif
}

#endif


//==============================================================================
// PlatformFileFinder
//==============================================================================
class PlatformFileFinder
	: public RefObject
{
public:
	PlatformFileFinder(const PlatformFileSystem::PathChar* dirPath, int dirPathLen, FileAttribute attr, const PlatformFileSystem::PathChar* pattern, int patternLen);
	~PlatformFileFinder();
	bool isWorking() const;
	const PlatformFileSystem::PathString& getCurrent() const;
	const PlatformFileSystem::PathString& getCurrentFileName() const { return m_impl.getCurrentFileName(); }
	FileAttribute getFileAttribute() const;
	bool next();

private:
	bool nextInternal();
	void setCurrent(const PlatformFileSystem::PathString& nativeCurrent);
	void clearCurrent();

	PlatformFileFinderImpl	m_impl;
	PlatformFileSystem::PathString m_dirPath;
	PlatformFileSystem::PathString m_current;
	FileAttribute m_attr;
	PlatformFileSystem::PathString	m_pattern;
};

PlatformFileFinder::PlatformFileFinder(const PlatformFileSystem::PathChar* dirPath, int dirPathLen, FileAttribute attr, const PlatformFileSystem::PathChar* pattern, int patternLen)
	: m_impl()
	, m_dirPath(dirPath)
	, m_current()
	, m_attr(attr)
	, m_pattern()
{
	if (pattern)
	{
		m_pattern.assign(pattern, patternLen);
	}

	m_current.reserve(m_dirPath.length() + 32);	// min size
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

const PlatformFileSystem::PathString& PlatformFileFinder::getCurrent() const
{
	return m_current;
}

FileAttribute PlatformFileFinder::getFileAttribute() const
{
	if (isWorking())
	{
		FileAttribute attr;
		if (PlatformFileSystem::getAttribute(getCurrent().c_str(), &attr))
		{
			return attr;
		}
	}
	return FileAttribute::None;
}

bool PlatformFileFinder::next()
{
	if (m_pattern.empty())
	{
		return nextInternal();
	}
	else
	{
		bool result = false;
		do
		{
			result = nextInternal();

		} while (result && !PlatformFileSystem::matchPath(getCurrent().c_str(), m_pattern.c_str()));

		return result;
	}
}

bool PlatformFileFinder::nextInternal()
{
	bool result = false;
	while (true)
	{
		result = m_impl.next();
		if (!result)
		{
			clearCurrent();
			break;
		}
		setCurrent(m_impl.getCurrentFileName());

		uint32_t attr = 0;
		FileAttribute a;
		if (PlatformFileSystem::getAttribute(getCurrent().c_str(), &a))
		{
			attr = a.getValue();
		}

		//uint32_t attr = detail::FileSystemInternal::getAttribute(getCurrent().c_str(), getCurrent().length()).getValue();
		uint32_t filter = (uint32_t)m_attr.getValue();
		if ((attr & filter) != 0)
		{
			break;
		}
	}

	return result;
}

void PlatformFileFinder::setCurrent(const PlatformFileSystem::PathString& nativeCurrent)
{
	m_current.assign(m_dirPath);
	m_current.append(1, (PlatformFileSystem::PathChar)PathTraits::DirectorySeparatorChar);
	m_current.append(m_impl.getCurrentFileName());
}

void PlatformFileFinder::clearCurrent()
{
	m_current.clear();
}

//==============================================================================
// FileSystem
//==============================================================================

CaseSensitivity FileSystem::getFileSystemCaseSensitivity()
{
#ifdef LN_OS_WIN32
	return CaseSensitivity::CaseInsensitive;
#else
	return CaseSensitivity::CaseSensitive;
#endif
}

bool FileSystem::existsFile(const StringRef& filePath)
{
	return detail::FileSystemInternal::existsFile(filePath.getBegin(), filePath.getLength());
}

FileAttribute FileSystem::getAttribute(const StringRef& filePath)
{
	return detail::FileSystemInternal::getAttribute(filePath.getBegin(), filePath.getLength());
}

void FileSystem::setAttribute(const StringRef& filePath, FileAttribute attr)
{
	detail::FileSystemInternal::setAttribute(filePath.getBegin(), filePath.getLength(), attr);
}

void FileSystem::copyFile(const StringRef& sourceFileName, const StringRef& destFileName, bool overwrite)
{
	detail::FileSystemInternal::copyFile(sourceFileName.getBegin(), sourceFileName.getLength(), destFileName.getBegin(), destFileName.getLength(), overwrite);
}

void FileSystem::deleteFile(const StringRef& filePath)
{
	detail::FileSystemInternal::deleteFile(filePath.getBegin(), filePath.getLength());
}

bool FileSystem::existsDirectory(const StringRef& path)
{
	FileAttribute attr = detail::FileSystemInternal::getAttribute(path.getBegin(), path.getLength());
	return attr.TestFlag(FileAttribute::Directory);
}

void FileSystem::createDirectory(const StringRef& path)
{
	detail::FileSystemInternal::createDirectory(path.getBegin(), path.getLength());
}

void FileSystem::deleteDirectory(const StringRef& path, bool recursive)
{
	detail::FileSystemInternal::deleteDirectory(path.getBegin(), path.getLength(), recursive);
}

void FileSystem::copyDirectory(const StringRef& srcPath, const StringRef& dstPath, bool overwrite, bool recursive)
{
	detail::FileSystemInternal::copyDirectory(srcPath.getBegin(), srcPath.getLength(), dstPath.getBegin(), dstPath.getLength(), overwrite, recursive);
}

void FileSystem::getCurrentDirectory(UString* outPath)
{
	PlatformFileSystem::PathChar path[LN_MAX_PATH];
	int len = PlatformFileSystem::getCurrentDirectory(LN_MAX_PATH, path);
	*outPath = UString::fromCString(path, len);
}

bool FileSystem::matchPath(const StringRef& filePath, const StringRef& pattern)
{
	return detail::FileSystemInternal::matchPath(filePath.getBegin(), filePath.getLength(), pattern.getBegin(), pattern.getLength());
}

uint64_t FileSystem::getFileSize(const StringRef& filePath)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath.getBegin(), filePath.getLength());
	return PlatformFileSystem::getFileSize(localPath.c_str());
}

uint64_t FileSystem::getFileSize(FILE* stream)
{
	return PlatformFileSystem::getFileSize(stream);
}


ByteBuffer FileSystem::readAllBytes(const StringRef& filePath)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath.getBegin(), filePath.getLength());
	const PlatformFileSystem::PathChar mode[] = { 'r', 'b', '\0' };
	FILE* fp = PlatformFileSystem::fopen(localPath.c_str(), mode);
	LN_THROW(fp, FileNotFoundException, localPath.c_str());

	size_t size = (size_t)getFileSize(fp);
	ByteBuffer buffer(size);
	fread(buffer.getData(), 1, size, fp);
	return buffer;
}
//ByteBuffer FileSystem::readAllBytes(const StringRefW& filePath)
//{
//	detail::GenericStaticallyLocalPath<wchar_t> localPath(filePath.getBegin(), filePath.getLength());
//	FILE* fp;
//	errno_t err = _wfopen_s(&fp, localPath.c_str(), L"rb");
//	LN_THROW(err == 0, FileNotFoundException, localPath.c_str());
//	size_t size = (size_t)getFileSize(fp);
//
//	ByteBuffer buffer(size);
//	fread(buffer.getData(), 1, size, fp);
//	return buffer;
//}

static String readAllTextHelper(const ByteBuffer& buffer, Encoding* encoding)
{
	if (encoding == nullptr)
	{
		Encoding* e = Encoding::getEncoding(EncodingType::UTF8);
		if (ByteBuffer::compare(buffer, e->getPreamble(), 3, 3) == 0)
			encoding = e;
		else
			encoding = Encoding::getUTF8Encoding();
	}

	return Encoding::fromBytes(buffer.getData(), buffer.getSize(), encoding);
}

//------------------------------------------------------------------------------
String FileSystem::readAllText(const StringRef& filePath, Encoding* encoding)
{
	ByteBuffer buffer(FileSystem::readAllBytes(filePath));
	return readAllTextHelper(buffer, encoding);
}

//------------------------------------------------------------------------------
String FileSystem::readAllText(Stream* stream, Encoding* encoding)
{
	ByteBuffer buffer(stream->getLength());
	stream->read(buffer.getData(), buffer.getSize());
	return readAllTextHelper(buffer, encoding);
}

//------------------------------------------------------------------------------
void FileSystem::writeAllBytes(const Char* filePath, const void* buffer, size_t size)
{
	Ref<FileStream> stream = FileStream::create(filePath, FileOpenMode::write | FileOpenMode::Truncate);
	stream->write(buffer, size);
}

//------------------------------------------------------------------------------
void FileSystem::writeAllText(const Char* filePath, const String& str, Encoding* encoding)
{
	encoding = (encoding == nullptr) ? Encoding::getUTF8Encoding() : encoding;

	EncodingConversionResult result;
	EncodingConversionOptions options;
	options.NullTerminated = false;
	ByteBuffer buffer = Encoding::convert(str.c_str(), str.getByteCount(), Encoding::getTCharEncoding(), encoding, options, &result);
	
	writeAllBytes(filePath, buffer.getConstData(), buffer.getSize());
}


//bool FileSystem::matchFileName(const TCHAR* filePath, const TCHAR* pattern)
//{
//	// TODO: Unix は fnmatch
//}




//==============================================================================
// FileSystemInternal
//==============================================================================
namespace detail {

bool FileSystemInternal::existsFile(const char* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	return PlatformFileSystem::existsFile(localPath.c_str());
}
bool FileSystemInternal::existsFile(const wchar_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	return PlatformFileSystem::existsFile(localPath.c_str());
}
bool FileSystemInternal::existsFile(const char16_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	return PlatformFileSystem::existsFile(localPath.c_str());
}

FileAttribute FileSystemInternal::getAttribute(const char* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	FileAttribute attr;
	if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
	return attr;
}
FileAttribute FileSystemInternal::getAttribute(const wchar_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	FileAttribute attr;
	bool r = PlatformFileSystem::getAttribute(localPath.c_str(), &attr);
	if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
	return attr;
}
FileAttribute FileSystemInternal::getAttribute(const char16_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	FileAttribute attr;
	bool r = PlatformFileSystem::getAttribute(localPath.c_str(), &attr);
	if (!PlatformFileSystem::getAttribute(localPath.c_str(), &attr)) return FileAttribute::None;
	return attr;
}


void FileSystemInternal::setAttribute(const char* filePath, int len, FileAttribute attr)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}
void FileSystemInternal::setAttribute(const wchar_t* filePath, int len, FileAttribute attr)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}
void FileSystemInternal::setAttribute(const char16_t* filePath, int len, FileAttribute attr)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::setAttribute(localPath.c_str(), attr);
}

void FileSystemInternal::copyFile(const char* sourceFileName, int sourceFileNameLen, const char* destFileName, int destFileNameLen, bool overwrite)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath1(sourceFileName, sourceFileNameLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath2(destFileName, destFileNameLen);
	PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), overwrite);
}
void FileSystemInternal::copyFile(const wchar_t* sourceFileName, int sourceFileNameLen, const wchar_t* destFileName, int destFileNameLen, bool overwrite)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath1(sourceFileName, sourceFileNameLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath2(destFileName, destFileNameLen);
	PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), overwrite);
}
void FileSystemInternal::copyFile(const char16_t* sourceFileName, int sourceFileNameLen, const char16_t* destFileName, int destFileNameLen, bool overwrite)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath1(sourceFileName, sourceFileNameLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath2(destFileName, destFileNameLen);
	PlatformFileSystem::copyFile(localPath1.c_str(), localPath2.c_str(), overwrite);
}

void FileSystemInternal::deleteFile(const char* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::deleteFile(localPath.c_str());
}
void FileSystemInternal::deleteFile(const wchar_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::deleteFile(localPath.c_str());
}
void FileSystemInternal::deleteFile(const char16_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	PlatformFileSystem::deleteFile(localPath.c_str());
}

bool FileSystemInternal::existsDirectory(const char* filePath, int len)
{
	FileAttribute attr = detail::FileSystemInternal::getAttribute(filePath, len);
	return attr.TestFlag(FileAttribute::Directory);
}
bool FileSystemInternal::existsDirectory(const wchar_t* filePath, int len)
{
	FileAttribute attr = detail::FileSystemInternal::getAttribute(filePath, len);
	return attr.TestFlag(FileAttribute::Directory);
}
bool FileSystemInternal::existsDirectory(const char16_t* filePath, int len)
{
	FileAttribute attr = detail::FileSystemInternal::getAttribute(filePath, len);
	return attr.TestFlag(FileAttribute::Directory);
}

static void createDirectoryInternal2(const PlatformFileSystem::PathChar* begin, const PlatformFileSystem::PathChar* end)
{
	std::vector<std::basic_string<PlatformFileSystem::PathChar>>	pathList;
	std::basic_string<PlatformFileSystem::PathChar> dir;

	int i = (end - begin) - 1;//StringTraits::tcslen(path) - 1;	// 一番後ろの文字の位置
	while (i >= 0)
	{
		dir.assign(begin, i + 1);

		FileAttribute attr;
		bool result = PlatformFileSystem::getAttribute(dir.c_str(), &attr);
		if (result && attr.TestFlag(FileAttribute::Directory))
		{
			break;
		}
		pathList.push_back(dir);

		// セパレータが見つかるまで探す
		while (i > 0 && begin[i] != PathTraits::DirectorySeparatorChar && begin[i] != PathTraits::AltDirectorySeparatorChar)
		{
			--i;
		}
		--i;
	}

	if (pathList.empty()) { return; }	// path が存在している

	for (int i = pathList.size() - 1; i >= 0; --i)
	{
		PlatformFileSystem::createDirectory(pathList[i].c_str());
	}
}
void FileSystemInternal::createDirectory(const char* path, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}
void FileSystemInternal::createDirectory(const wchar_t* path, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}
void FileSystemInternal::createDirectory(const char16_t* path, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	createDirectoryInternal2(localPath.c_str(), localPath.c_str() + localPath.getLength());
}

static void deleteDirectoryInternal(const PlatformFileSystem::PathChar* path, int len, bool recursive)
{
	if (recursive)
	{
		PlatformFileFinder finder(path, len, FileAttribute::All, nullptr, 0);
		while (finder.isWorking())
		{
			auto& current = finder.getCurrent();
			if (finder.getFileAttribute().TestFlag(FileAttribute::Directory))
			{
				deleteDirectoryInternal(current.c_str(), current.length(), recursive);	// recursive
			}
			else // TODO: 他の属性みないとダメ。シンボリックリンクとか
			{
				PlatformFileSystem::deleteFile(current.c_str());
			}
			finder.next();
		}
	}
	PlatformFileSystem::removeDirectory(path);
}
void FileSystemInternal::deleteDirectory(const char* path, int len, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}
void FileSystemInternal::deleteDirectory(const wchar_t* path, int len, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}
void FileSystemInternal::deleteDirectory(const char16_t* path, int len, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, len);
	deleteDirectoryInternal(localPath.c_str(), localPath.getLength(), recursive);
}

static void copyDirectoryInternal(
	const PlatformFileSystem::PathChar* srcPath,
	const PlatformFileSystem::PathChar* dstPath,
	bool overwrite, bool recursive)
{
	// 上書きしないとき、すでにフォルダが存在してはならない
	if (!overwrite)
	{
		FileAttribute attr;
		if (PlatformFileSystem::getAttribute(srcPath, &attr) &&
			attr.TestFlag(FileAttribute::Directory))
		{
			LN_THROW(0, IOException);
		}
	}

	// コピー先フォルダを作っておく
	PlatformFileSystem::createDirectory(dstPath);

	PlatformFileFinder finder(srcPath, StringTraits::tcslen(srcPath), FileAttribute::All, nullptr, 0);
	PlatformFileSystem::PathString dest;
	while (finder.isWorking())
	{
		const PlatformFileSystem::PathString& src = finder.getCurrent();
		dest.assign(dstPath);
		dest.append(1, PathTraits::DirectorySeparatorChar);
		dest.append(finder.getCurrentFileName());

		FileAttribute srcAttr;
		if (!PlatformFileSystem::getAttribute(src.c_str(), &srcAttr)) srcAttr = FileAttribute::None;

		FileAttribute dstAttr;
		if (!PlatformFileSystem::getAttribute(dest.c_str(), &dstAttr)) dstAttr = FileAttribute::None;

		if (srcAttr.TestFlag(FileAttribute::Normal))
		{
			if (dstAttr.TestFlag(FileAttribute::Directory))
			{
				// TODO: src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
				LN_NOTIMPLEMENTED();
			}
			else if (dstAttr.TestFlag(FileAttribute::Normal))
			{
				// コピー先にファイルとして存在していて、上書きする場合はコピーする
				if (overwrite)
				{
					PlatformFileSystem::copyFile(src.c_str(), dest.c_str(), true);
				}
			}
			else
			{
				PlatformFileSystem::copyFile(src.c_str(), dest.c_str(), false);
			}
		}
		else if (srcAttr.TestFlag(FileAttribute::Directory))
		{
			if (dstAttr.TestFlag(FileAttribute::Normal))
			{
				// TODO: src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
				LN_NOTIMPLEMENTED();
			}

			if (recursive)
			{
				copyDirectoryInternal(src.c_str(), dest.c_str(), overwrite, recursive);
			}
		}

		finder.next();
	}
}
void FileSystemInternal::copyDirectory(const char* srcPath, int srcPathLen, const char* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localSrcPath(srcPath, srcPathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localDstPath(dstPath, dstPathLen);
	copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
void FileSystemInternal::copyDirectory(const wchar_t* srcPath, int srcPathLen, const wchar_t* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localSrcPath(srcPath, srcPathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localDstPath(dstPath, dstPathLen);
	copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
void FileSystemInternal::copyDirectory(const char16_t* srcPath, int srcPathLen, const char16_t* dstPath, int dstPathLen, bool overwrite, bool recursive)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localSrcPath(srcPath, srcPathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localDstPath(dstPath, dstPathLen);
	copyDirectoryInternal(localSrcPath.c_str(), localDstPath.c_str(), overwrite, recursive);
}
bool FileSystemInternal::matchPath(const char* path, int pathLen, const char* pattern, int patternLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPattern(pattern, patternLen);
	return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}
bool FileSystemInternal::matchPath(const wchar_t* path, int pathLen, const wchar_t* pattern, int patternLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPattern(pattern, patternLen);
	return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}
bool FileSystemInternal::matchPath(const char16_t* path, int pathLen, const char16_t* pattern, int patternLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPattern(pattern, patternLen);
	return PlatformFileSystem::matchPath(localPath.c_str(), localPattern.c_str());
}

FILE* FileSystemInternal::fopen(const char* path, int pathLen, const char* mode, int modeLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localMode(mode, modeLen);
	return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}
FILE* FileSystemInternal::fopen(const wchar_t* path, int pathLen, const wchar_t* mode, int modeLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localMode(mode, modeLen);
	return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}
FILE* FileSystemInternal::fopen(const char16_t* path, int pathLen, const char16_t* mode, int modeLen)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(path, pathLen);
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localMode(mode, modeLen);
	return PlatformFileSystem::fopen(localPath.c_str(), localMode.c_str());
}

// 現在の位置とデータ(ファイル)サイズ、オフセット、基準(SEEK_xxxx)を受け取って、新しいシーク位置を返す
int64_t FileSystemInternal::calcSeekPoint(int64_t curPoint, int64_t maxSize, int64_t offset, int origin)
{
	int64_t newPoint = curPoint;
	switch (origin)
	{
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

} // namespace detail





class DirectoryIterator
{
public:
	using value_type = PathName;

	DirectoryIterator()
		: m_finder(nullptr)
		, m_path()
	{}

	DirectoryIterator(const StringRef dirPath, FileAttribute attr = FileAttribute::All, const StringRef pattern = nullptr)
		: m_finder()
		, m_path()
	{
		detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(dirPath.getBegin(), dirPath.getLength());
		detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPattern(pattern.getBegin(), pattern.getLength());
		m_finder = Ref<PlatformFileFinder>::makeRef(localPath.c_str(), localPath.getLength(), attr, localPattern.c_str(), localPattern.getLength());
		m_path = m_finder->getCurrent().c_str();
	}

	DirectoryIterator(const DirectoryIterator& other)
		: m_finder(other.m_finder)
		, m_path(other.m_path)
	{
	}

	DirectoryIterator& operator = (const DirectoryIterator& other)
	{
		m_finder = other.m_finder;
		m_path = other.m_path;
	}

	DirectoryIterator& operator ++ ()   // prefix
	{
		if (m_finder != nullptr)
		{
			m_finder->next();
			m_path = m_finder->getCurrent().c_str();
		}
		return *this;
	}

	DirectoryIterator operator ++ (int) // postfix
	{
		if (m_finder != nullptr)
		{
			m_finder->next();
			m_path = m_finder->getCurrent().c_str();
		}
		return *this;
	}

	const PathName& operator * () const { return m_path; }
	PathName& operator * () { return m_path; }
	const PathName* operator -> () const { return &m_path; }
	PathName* operator -> () { return &m_path; }

	bool operator == (const DirectoryIterator& othre) const { return m_path == othre.m_path; }
	bool operator != (const DirectoryIterator& othre) const { return m_path != othre.m_path; }

private:
	Ref<PlatformFileFinder>			m_finder;
	PathName						m_path;
};

tr::Enumerator<PathName> FileSystem::getFiles(const StringRef& dirPath, const StringRef& pattern)
{
	DirectoryIterator itr(dirPath, FileAttribute::Normal, pattern.IsNullOrEmpty() ? nullptr : pattern.getBegin());
	DirectoryIterator end;
	return tr::MakeEnumerator::from(itr, end);
}

LN_NAMESPACE_END
