
#include <sys/types.h>
#include <sys/stat.h>
#include <list>
#include "Internal.hpp"
#include <Lumino/Base/Buffer.hpp>
#include <Lumino/Text/Encoding.hpp>
//#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/FileSystem.hpp>
//#include <Lumino/IO/Path.h>
#if defined(LN_OS_WIN32)
#include "FileSystem_Win32.hpp"
#else
#include "FileSystem_Unix.hpp"
#endif

namespace ln {
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
	bool isFirst() const;
	const PlatformFileSystem::PathString& getCurrent() const;
	const PlatformFileSystem::PathString& getCurrentFileName() const { return m_impl.getCurrentFileName(); }
	Flags<FileAttribute> getFileAttribute() const;
	bool next();

private:
	bool nextInternal(bool* outIsDir);
	void setCurrent(const PlatformFileSystem::PathString& nativeCurrent);
	void clearCurrent();

	PlatformFileFinderImpl	m_impl;
	PlatformFileSystem::PathString m_dirPath;
	PlatformFileSystem::PathString m_current;
	Flags<FileAttribute> m_attr;
	PlatformFileSystem::PathString	m_pattern;
	bool m_first;
};

PlatformFileFinder::PlatformFileFinder(const PlatformFileSystem::PathChar* dirPath, int dirPathLen, FileAttribute attr, const PlatformFileSystem::PathChar* pattern, int patternLen)
	: m_impl()
	, m_dirPath(dirPath)
	, m_current()
	, m_attr(attr)
	, m_pattern()
	, m_first(true)
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

bool PlatformFileFinder::isFirst() const
{
	return m_first;
}

const PlatformFileSystem::PathString& PlatformFileFinder::getCurrent() const
{
	return m_current;
}

Flags<FileAttribute> PlatformFileFinder::getFileAttribute() const
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
	bool isDir = false;
	if (m_pattern.empty())
	{
		return nextInternal(&isDir);
	}
	else
	{
		bool result = false;
		do
		{
			result = nextInternal(&isDir);
			//if (result)
			//{
			//	if (1 &&	// TODO: Dir を列挙するのか、File を列挙するのか。今はとりあえず File だけ
			//		!isDir)
			//	{
			//	}
			//}

		} while (result && !PlatformFileSystem::matchPath(getCurrent().c_str(), m_pattern.c_str()));

		return result;
	}
}

bool PlatformFileFinder::nextInternal(bool* outIsDir)
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

		FileAttribute attr = FileAttribute::None;
		FileAttribute a;
		if (PlatformFileSystem::getAttribute(getCurrent().c_str(), &a))
		{
			attr = a;
		}

		*outIsDir = ((a & FileAttribute::Directory) != FileAttribute::None);

		auto filter = m_attr;
		if ((attr & filter) != FileAttribute::None)
		{
			break;
		}
	}

	m_first = false;
	return result;
}

void PlatformFileFinder::setCurrent(const PlatformFileSystem::PathString& nativeCurrent)
{
	m_current.assign(m_dirPath);
	m_current.append(1, (PlatformFileSystem::PathChar)detail::PathTraits::DirectorySeparatorChar);
	m_current.append(m_impl.getCurrentFileName());
}

void PlatformFileFinder::clearCurrent()
{
	m_current.clear();
}

//==============================================================================
// FileSystem
//==============================================================================


bool FileSystem::existsFile(const StringRef& filePath)
{
	return detail::FileSystemInternal::existsFile(filePath.getBegin(), filePath.length());
}

FileAttribute FileSystem::getAttribute(const StringRef& filePath)
{
	return detail::FileSystemInternal::getAttribute(filePath.getBegin(), filePath.length());
}

void FileSystem::setAttribute(const StringRef& filePath, FileAttribute attr)
{
	detail::FileSystemInternal::setAttribute(filePath.getBegin(), filePath.length(), attr);
}

void FileSystem::copyFile(const StringRef& sourceFileName, const StringRef& destFileName, bool overwrite)
{
	detail::FileSystemInternal::copyFile(sourceFileName.getBegin(), sourceFileName.length(), destFileName.getBegin(), destFileName.length(), overwrite);
}

void FileSystem::deleteFile(const StringRef& filePath)
{
	detail::FileSystemInternal::deleteFile(filePath.getBegin(), filePath.length());
}

bool FileSystem::existsDirectory(const StringRef& path)
{
	Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(path.getBegin(), path.length());
	return attr.hasFlag(FileAttribute::Directory);
}

void FileSystem::createDirectory(const StringRef& path)
{
	detail::FileSystemInternal::createDirectory(path.getBegin(), path.length());
}

void FileSystem::deleteDirectory(const StringRef& path, bool recursive)
{
	detail::FileSystemInternal::deleteDirectory(path.getBegin(), path.length(), recursive);
}

void FileSystem::copyDirectory(const StringRef& srcPath, const StringRef& dstPath, bool overwrite, bool recursive)
{
	detail::FileSystemInternal::copyDirectory(srcPath.getBegin(), srcPath.length(), dstPath.getBegin(), dstPath.length(), overwrite, recursive);
}

//void FileSystem::getCurrentDirectory(String* outPath)
//{
//	PlatformFileSystem::PathChar path[MAX_PATH];
//	int len = PlatformFileSystem::getCurrentDirectory(MAX_PATH, path);
//	*outPath = String::fromCString(path, len);
//}

bool FileSystem::matchPath(const StringRef& filePath, const StringRef& pattern)
{
	return detail::FileSystemInternal::matchPath(filePath.getBegin(), filePath.length(), pattern.getBegin(), pattern.length());
}

uint64_t FileSystem::getFileSize(const StringRef& filePath)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath.getBegin(), filePath.length());
	return PlatformFileSystem::getFileSize(localPath.c_str());
}

uint64_t FileSystem::getFileSize(FILE* stream)
{
	return PlatformFileSystem::getFileSize(stream);
}


ByteBuffer FileSystem::readAllBytes(const StringRef& filePath)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath.getBegin(), filePath.length());
	const PlatformFileSystem::PathChar mode[] = { 'r', 'b', '\0' };
	FILE* fp = PlatformFileSystem::fopen(localPath.c_str(), mode);
	if (LN_ENSURE_IO(fp, localPath.c_str())) return ByteBuffer();

	size_t size = (size_t)getFileSize(fp);
	ByteBuffer buffer(size);
	fread(buffer.getData(), 1, size, fp);

	fclose(fp);
	LN_EMSCRIPTEN_LAYZY_FLASH;
	return buffer;
}
//ByteBuffer FileSystem::readAllBytes(const StringRefW& filePath)
//{
//	detail::GenericStaticallyLocalPath<wchar_t> localPath(filePath.getBegin(), filePath.length());
//	FILE* fp;
//	errno_t err = _wfopen_s(&fp, localPath.c_str(), L"rb");
//	LN_THROW(err == 0, FileNotFoundException, localPath.c_str());
//	size_t size = (size_t)getFileSize(fp);
//
//	ByteBuffer buffer(size);
//	fread(buffer.getData(), 1, size, fp);
//	return buffer;
//}

static String readAllTextHelper(const ByteBuffer& buffer, TextEncoding* encoding)
{
	if (encoding == nullptr)
	{
		TextEncoding* e = TextEncoding::getEncoding(EncodingType::UTF8);
		if (ByteBuffer::compare(buffer, e->getPreamble(), 3, 3) == 0)
			encoding = e;
		else
			encoding = TextEncoding::utf8Encoding();
	}

	return encoding->decode(buffer.getData(), buffer.getSize());
}

//------------------------------------------------------------------------------
String FileSystem::readAllText(const StringRef& filePath, TextEncoding* encoding)
{
	ByteBuffer buffer(FileSystem::readAllBytes(filePath));
	return readAllTextHelper(buffer, encoding);
}

//------------------------------------------------------------------------------
void FileSystem::writeAllBytes(const StringRef& filePath, const void* buffer, size_t size)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath.getBegin(), filePath.length());
	const PlatformFileSystem::PathChar mode[] = { 'w', 'b', '\0' };
	FILE* fp = PlatformFileSystem::fopen(localPath.c_str(), mode);
	if (LN_ENSURE(fp != NULL, localPath.c_str())) return;

	fwrite(buffer, 1, size, fp);
	fclose(fp);
	LN_EMSCRIPTEN_LAYZY_FLASH;
}

//------------------------------------------------------------------------------
void FileSystem::writeAllText(const Char* filePath, const String& str, TextEncoding* encoding)
{
	encoding = (encoding == nullptr) ? TextEncoding::utf8Encoding() : encoding;

	auto buffer = encoding->encode(str);
	
	writeAllBytes(filePath, buffer.data(), buffer.size());
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
	if (PlatformFileSystem::existsFile(localPath.c_str())) {
		PlatformFileSystem::deleteFile(localPath.c_str());
	}
}
void FileSystemInternal::deleteFile(const wchar_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	if (PlatformFileSystem::existsFile(localPath.c_str())) {
		PlatformFileSystem::deleteFile(localPath.c_str());
	}
}
void FileSystemInternal::deleteFile(const char16_t* filePath, int len)
{
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(filePath, len);
	if (PlatformFileSystem::existsFile(localPath.c_str())) {
		PlatformFileSystem::deleteFile(localPath.c_str());
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
bool FileSystemInternal::existsDirectory(const char16_t* filePath, int len)
{
	Flags<FileAttribute> attr = detail::FileSystemInternal::getAttribute(filePath, len);
	return attr.hasFlag(FileAttribute::Directory);
}

static void createDirectoryInternal2(const PlatformFileSystem::PathChar* begin, const PlatformFileSystem::PathChar* end)
{
	if (FileSystemInternal::existsDirectory(begin, (end - begin))) return;

	std::vector<std::basic_string<PlatformFileSystem::PathChar>>	pathList;
	std::basic_string<PlatformFileSystem::PathChar> dir;

	int i = (end - begin) - 1;//StringTraits::tcslen(path) - 1;	// 一番後ろの文字の位置
	while (i >= 0)
	{
		dir.assign(begin, i + 1);

		FileAttribute attr;
		bool result = PlatformFileSystem::getAttribute(dir.c_str(), &attr);
		if (result && testFlag(attr, FileAttribute::Directory))
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
			if (finder.getFileAttribute().hasFlag(FileAttribute::Directory))
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
			testFlag(attr, FileAttribute::Directory))
		{
			LN_ENSURE_IO(0);
			return;
		}
	}

	// コピー先フォルダを作っておく
	PlatformFileSystem::createDirectory(dstPath);

	PlatformFileFinder finder(srcPath, StringHelper::strlen(srcPath), FileAttribute::All, nullptr, 0);
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

		if (testFlag(srcAttr, FileAttribute::Normal))
		{
			if (testFlag(dstAttr, FileAttribute::Directory))
			{
				// TODO: src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
				LN_NOTIMPLEMENTED();
			}
			else if (testFlag(dstAttr, FileAttribute::Normal))
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
		else if (testFlag(srcAttr, FileAttribute::Directory))
		{
			if (testFlag(dstAttr, FileAttribute::Normal))
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





//class DirectoryIterator
//{
//public:
//	using value_type = Path;
//
//	DirectoryIterator()
//		: m_finder(nullptr)
//		, m_path()
//	{}
//
//	DirectoryIterator(const StringRef dirPath, FileAttribute attr = FileAttribute::All, const StringRef pattern = StringRef())
//		: m_finder()
//		, m_path()
//	{
//		detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(dirPath.getBegin(), dirPath.length());
//		detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPattern(pattern.getBegin(), pattern.length());
//		m_finder = Ref<PlatformFileFinder>::makeRef(localPath.c_str(), localPath.getLength(), attr, localPattern.c_str(), localPattern.getLength());
//		m_path = String::fromStdString(m_finder->getCurrent());
//	}
//
//	DirectoryIterator(const DirectoryIterator& other)
//		: m_finder(other.m_finder)
//		, m_path(other.m_path)
//	{
//	}
//
//	DirectoryIterator& operator = (const DirectoryIterator& other)
//	{
//		m_finder = other.m_finder;
//		m_path = other.m_path;
//		return *this;
//	}
//
//	DirectoryIterator& operator ++ ()   // prefix
//	{
//		if (m_finder != nullptr)
//		{
//			m_finder->next();
//			m_path = String::fromStdString(m_finder->getCurrent());
//		}
//		return *this;
//	}
//
//	DirectoryIterator operator ++ (int) // postfix
//	{
//		if (m_finder != nullptr)
//		{
//			m_finder->next();
//			m_path = String::fromStdString(m_finder->getCurrent());
//		}
//		return *this;
//	}
//
//	const Path& operator * () const { return m_path; }
//	Path& operator * () { return m_path; }
//	const Path* operator -> () const { return &m_path; }
//	Path* operator -> () { return &m_path; }
//
//	bool operator == (const DirectoryIterator& othre) const { return m_path == othre.m_path; }
//	bool operator != (const DirectoryIterator& othre) const { return m_path != othre.m_path; }
//
//private:
//	Ref<PlatformFileFinder>			m_finder;
//	Path						m_path;
//};

//tr::Enumerator<Path> FileSystem::getFiles(const StringRef& dirPath, const StringRef& pattern)
//{
//	DirectoryIterator itr(dirPath, FileAttribute::Normal, pattern.IsNullOrEmpty() ? nullptr : pattern.getBegin());
//	DirectoryIterator end;
//	return tr::MakeEnumerator::from(itr, end);
//}





namespace detail {

class DirectoryIterator2Impl
	: public RefObject
{
public:
	DirectoryIterator2Impl(const StringRef& path, const StringRef& searchPattern, SearchOption searchOption, SearchTargetEntity targetEntity)
		: m_path(path)
		, m_searchPattern(searchPattern.getBegin(), searchPattern.length())	// TODO: check
		//, m_filterAttr(FileAttribute::All)
		, m_searchTargetEntity(targetEntity)	// TODO:
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
		do
		{
			if (setup && first)
			{
				result = current()->isWorking();
			}
			else
			{
				result = nextInternal(setup);
			}
			
			setup = first = false;

			if (result)
			{
				if (m_searchTargetEntity == SearchTargetEntity::Directory &&
					current()->getFileAttribute().hasFlag(FileAttribute::Directory))
				{
					if (m_searchPattern.isEmpty())
					{
						break;	// パターン指定なし。返す。
					}
					if (PlatformFileSystem::matchPath(current()->getCurrent().c_str(), m_searchPattern.c_str()))
					{
						break;	// パターンに一致。返す。
					}
				}
				if (m_searchTargetEntity == SearchTargetEntity::File &&
					!current()->getFileAttribute().hasFlag(FileAttribute::Directory))
				{
					if (m_searchPattern.isEmpty())
					{
						break;	// パターン指定なし。返す。
					}
					if (PlatformFileSystem::matchPath(current()->getCurrent().c_str(), m_searchPattern.c_str()))
					{
						break;	// パターンに一致。返す。
					}
				}
			}

		} while (result/* && !m_filterAttr.TestFlag((FileAttribute::enum_type)current()->getFileAttribute().getValue())*/);

		if (result)
		{
			m_currentPath = String::fromStdString(current()->getCurrent());
		}
		else
		{
			m_currentPath.clear();
		}

		return result;
	}

	bool nextInternal(bool setup)
	{
		if (m_searchOption == SearchOption::Recursive &&
			current()->getFileAttribute().hasFlag(FileAttribute::Directory))
		{
			pushFilder();
		}
		else
		{
			if (!setup)
				current()->next();
		}

		while (!current()->isWorking())
		{
			popFinder();		// 戻す
			if (m_stack.empty()) return false;

			current()->next();	// 戻したフォルダを見ているので1つ進める
		}
		return current()->isWorking();
	}

	// 関数を抜けたとき、stack top は既に次に返すべき Entity を指している
	// または、Entity が一つもなければ isWorkding が false になっている
	void pushFilder()
	{
		if (m_stack.empty())
		{
			detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> localPath(m_path.c_str(), m_path.length());
			m_stack.push_back(Ref<PlatformFileFinder>::makeRef(localPath.c_str(), localPath.getLength(), FileAttribute::All, nullptr, 0/*, m_filterAttr | FileAttribute::Directory, m_searchPattern.c_str(), m_searchPattern.length()*/));
		}
		else
		{
			m_stack.push_back(Ref<PlatformFileFinder>::makeRef(m_stack.back()->getCurrent().c_str(), m_stack.back()->getCurrent().length(), FileAttribute::All, nullptr, 0/*, m_filterAttr | FileAttribute::Directory, m_searchPattern.c_str(), m_searchPattern.length()*/));
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
	detail::GenericStaticallyLocalPath<PlatformFileSystem::PathChar> m_searchPattern;
	SearchTargetEntity m_searchTargetEntity;
	SearchOption m_searchOption;
	std::list<Ref<PlatformFileFinder>> m_stack;
	Path m_currentPath;
};




DirectoryIterator2::DirectoryIterator2()
	: m_impl()
{}

DirectoryIterator2::DirectoryIterator2(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption, SearchTargetEntity targetEntity)
	: m_impl()
{
	m_impl = Ref<DirectoryIterator2Impl>::makeRef(dirPath, pattern, searchOption, targetEntity);
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

DirectoryIterator2& DirectoryIterator2::operator = (const DirectoryIterator2& other)
{
	m_impl = other.m_impl;
	m_current = other.m_current;
	return *this;
}

DirectoryIterator2& DirectoryIterator2::operator ++ ()   // prefix
{
	if (m_impl != nullptr)
	{
		m_impl->next();
		m_current = m_impl->currentPath();
	}
	return *this;
}

DirectoryIterator2 DirectoryIterator2::operator ++ (int) // postfix
{
	if (m_impl != nullptr)
	{
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

DirectoryIteratorRange FileSystem::getFiles(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption)
{
	return DirectoryIteratorRange(dirPath, pattern, searchOption, detail::SearchTargetEntity::File);
}

DirectoryIteratorRange FileSystem::getDirectories(const StringRef& dirPath, const StringRef& pattern, SearchOption searchOption)
{
	return DirectoryIteratorRange(dirPath, pattern, searchOption, detail::SearchTargetEntity::Directory);
}


} // namespace ln
