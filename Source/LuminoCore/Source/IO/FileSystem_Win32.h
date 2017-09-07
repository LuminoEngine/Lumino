
#include <sys/types.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include "../Internal.h"
#include "../../include/Lumino/Text/Encoding.h"
#include "../../include/Lumino/IO/FileStream.h"
#include "../../include/Lumino/IO/FileSystem.h"
#include "../../include/Lumino/IO/PathTraits.h"

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
template<typename TChar>
static void Win32IOErrorToExceptionThrow(DWORD errorCode, const TChar* message)
{
	switch (errorCode) {
	case ERROR_FILE_NOT_FOUND:
		LN_THROW(0, FileNotFoundException, message);

	case ERROR_PATH_NOT_FOUND:
		LN_THROW(0, DirectoryNotFoundException, message);

	case ERROR_ACCESS_DENIED:
		LN_THROW(0, IOException, LN_T(TChar, "Unauthorized access : %s"), message);

	case ERROR_ALREADY_EXISTS:
		LN_THROW(0, IOException, LN_T(TChar, "Already exists : %s"), message);

	case ERROR_FILENAME_EXCED_RANGE:
		LN_THROW(0, IOException, LN_T(TChar, "Path too long : %s"), message);

	case ERROR_INVALID_DRIVE:
		LN_THROW(0, IOException, LN_T(TChar, "Drive not found : %s"), message);

	case ERROR_FILE_EXISTS:
		LN_THROW(0, IOException, LN_T(TChar, "File exists : %s"), message);

	default:
		LN_THROW(0, Win32Exception, errorCode);
	}
}





class PlatformFileSystem
{
public:
	using PathChar = wchar_t;
	using PathString = std::wstring;

	static bool existsFile(const wchar_t* filePath)
	{
		// ※fopen によるチェックはNG。ファイルが排他ロックで開かれていた時に失敗する。
		DWORD attr = ::GetFileAttributesW(filePath);
		// 他ユーザーフォルダ内のファイルにアクセスしようとすると attr = -1 になる。
		// このとき GetLastError() は ERROR_ACCESS_DENIED である。
		// .NET の仕様にあわせ、エラーは一律 false で返している。
		return ((attr != -1) && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
	}

	static void setAttribute(const wchar_t* filePath, FileAttribute attr)
	{
		DWORD dwAttr = 0;
		if (attr.TestFlag(FileAttribute::Directory)) dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
		if (attr.TestFlag(FileAttribute::ReadOnly))  dwAttr |= FILE_ATTRIBUTE_READONLY;
		if (attr.TestFlag(FileAttribute::Hidden))    dwAttr |= FILE_ATTRIBUTE_HIDDEN;
		BOOL r = ::SetFileAttributesW(filePath, dwAttr);
		if (r == FALSE) { Win32IOErrorToExceptionThrow(::GetLastError(), filePath); }
	}

	static bool getAttribute(const wchar_t* path, FileAttribute* outAttr)
	{
		DWORD attr = ::GetFileAttributesW(path);
		if (attr == -1) { return false; }

		FileAttribute flags = FileAttribute::None;
		if (attr & FILE_ATTRIBUTE_DIRECTORY)	flags |= FileAttribute::Directory;
		else									flags |= FileAttribute::Normal;
		if (attr & FILE_ATTRIBUTE_READONLY)		flags |= FileAttribute::ReadOnly;
		if (attr & FILE_ATTRIBUTE_HIDDEN)		flags |= FileAttribute::Hidden;
		*outAttr = flags;
		return true;
	}

	static void copyFile(const wchar_t* sourceFileName, const wchar_t* destFileName, bool overwrite)
	{
		BOOL bRes = ::CopyFileW(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
		if (bRes == FALSE) {
			// TODO 引数もう一つ増やさないと、どちらが原因かわかりにくい
			Win32IOErrorToExceptionThrow(::GetLastError(), sourceFileName);
		}
	}

	static void deleteFile(const wchar_t* filePath)
	{
		BOOL r = ::DeleteFileW(filePath);
		if (r == FALSE) {
			Win32IOErrorToExceptionThrow(::GetLastError(), filePath);
		}
	}

	static bool createDirectory(const wchar_t* path)
	{
		return ::CreateDirectoryW(path, NULL) != FALSE;
	}

	static void removeDirectory(const wchar_t* path)
	{
		BOOL r = ::RemoveDirectoryW(path);
		if (r == FALSE) {
			Win32IOErrorToExceptionThrow(::GetLastError(), path);
		}
	}

	static bool matchPath(const wchar_t* filePath, const wchar_t* pattern)
	{
		return ::PathMatchSpecExW(filePath, pattern, PMSF_NORMAL) == S_OK;
	}

	static int getCurrentDirectory(int bufferLength, wchar_t* outBuffer)
	{
		return ::GetCurrentDirectoryW(bufferLength, outBuffer);
	}
	
	static uint64_t getFileSize(const wchar_t* filePath)
	{
		struct _stat stat_buf;
		int r = _wstat(filePath, &stat_buf);
		if (r != 0) return 0;
		return stat_buf.st_size;
	}

	static uint64_t getFileSize(FILE* stream)
	{
		struct _stat stbuf;
		int handle = _fileno(stream);
		if (handle == 0) return 0;
		if (_fstat(handle, &stbuf) == -1) return 0;
		return stbuf.st_size;
	}

	static FILE* fopen(const wchar_t* path, const wchar_t* mode)
	{
		FILE* fp;
		if (_wfopen_s(&fp, path, mode) != 0) return 0;
		return fp;
	}
};

class PlatformFileFinderImpl
{
public:
	PlatformFileFinderImpl()
		: m_fh(INVALID_HANDLE_VALUE)
		, m_fd()
		, m_current()
	{}

	~PlatformFileFinderImpl()
	{
		if (m_fh != INVALID_HANDLE_VALUE)
		{
			::FindClose(m_fh);
		}
	}

	void initialize(const wchar_t* path, int len)
	{
		std::wstring pattern;
		pattern.reserve(len + 2);
		pattern.append(path, len);
		if (!PathTraits::isSeparatorChar(pattern[len]))
		{
			pattern.append(L"\\");
		}
		pattern.append(L"*");

		m_fh = ::FindFirstFileW(pattern.c_str(), &m_fd);
		LN_THROW(m_fh != INVALID_HANDLE_VALUE, IOException, path);
	}

	const std::wstring& getCurrentFileName() const
	{
		return m_current;
	}

	bool isWorking() const
	{
		return !m_current.empty();
	}

	bool next()
	{
		do
		{
			if (::FindNextFileW(m_fh, &m_fd) != 0)
			{
				m_current = m_fd.cFileName;
			}
			else
			{
				m_current.clear();
				break;
			}

		} while (wcscmp(m_fd.cFileName, L".") == 0 || wcscmp(m_fd.cFileName, L"..") == 0);

		return !m_current.empty();
	}

private:
	HANDLE				m_fh;
	WIN32_FIND_DATAW	m_fd;
	std::wstring		m_current;
};


//------------------------------------------------------------------------------
//bool FileSystem::existsFile(const StringRefA& filePath)
//{
//	if (filePath.IsNullOrEmpty()) return false;
//
//	char tmpPath[MAX_PATH];
//	filePath.copyTo(tmpPath, MAX_PATH);
//
//	// ※fopen によるチェックはNG。ファイルが排他ロックで開かれていた時に失敗する。
//	DWORD attr = ::GetFileAttributesA(tmpPath);
//	// 他ユーザーフォルダ内のファイルにアクセスしようとすると attr = -1 になる。
//	// このとき GetLastError() は ERROR_ACCESS_DENIED である。
//	// .NET の仕様にあわせ、エラーは一律 false で返している。
//	return ((attr != -1) &&
//			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
//}
//
//bool FileSystem::existsFile(const StringRefW& filePath)
//{
//	if (filePath.IsNullOrEmpty()) return false;
//
//	wchar_t tmpPath[MAX_PATH];
//	filePath.copyTo(tmpPath, MAX_PATH);
//
//	DWORD attr = ::GetFileAttributesW(tmpPath);
//	return ((attr != -1) &&
//			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
//}

////------------------------------------------------------------------------------
//void FileSystem::setAttribute(const char* filePath, FileAttribute attr)
//{
//	DWORD dwAttr = 0;
//	if (attr.TestFlag(FileAttribute::Directory)) dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
//	if (attr.TestFlag(FileAttribute::ReadOnly))  dwAttr |= FILE_ATTRIBUTE_READONLY;
//	if (attr.TestFlag(FileAttribute::Hidden))    dwAttr |= FILE_ATTRIBUTE_HIDDEN;
//	BOOL r = ::SetFileAttributesA(filePath, dwAttr);
//	if (r == FALSE) { Win32IOErrorToExceptionThrow(::GetLastError(), filePath); }
//}
//
//void FileSystem::setAttribute(const wchar_t* filePath, FileAttribute attr)
//{
//	DWORD dwAttr = 0;
//	if (attr.TestFlag(FileAttribute::Directory)) dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
//	if (attr.TestFlag(FileAttribute::ReadOnly))  dwAttr |= FILE_ATTRIBUTE_READONLY;
//	if (attr.TestFlag(FileAttribute::Hidden))    dwAttr |= FILE_ATTRIBUTE_HIDDEN;
//	BOOL r = ::SetFileAttributesW(filePath, dwAttr);
//	if (r == FALSE) { Win32IOErrorToExceptionThrow(::GetLastError(), filePath); }
//}
//
////------------------------------------------------------------------------------
//void FileSystem::copy(const char* sourceFileName, const char* destFileName, bool overwrite)
//{
//	BOOL bRes = ::CopyFileA(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
//	if (bRes == FALSE) {
//		// TODO 引数もう一つ増やさないと、どちらが原因かわかりにくい
//		Win32IOErrorToExceptionThrow(::GetLastError(), sourceFileName);
//	}
//}
//
//void FileSystem::copy(const wchar_t* sourceFileName, const wchar_t* destFileName, bool overwrite)
//{
//	BOOL bRes = ::CopyFileW(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
//	if (bRes == FALSE) {
//		// TODO 引数もう一つ増やさないと、どちらが原因かわかりにくい
//		Win32IOErrorToExceptionThrow(::GetLastError(), sourceFileName);
//	}
//}

////------------------------------------------------------------------------------
//void FileSystem::deleteFile(const char* filePath)
//{
//	BOOL r = ::DeleteFileA(filePath);
//	if (r == FALSE) {
//		Win32IOErrorToExceptionThrow(::GetLastError(), filePath);
//	}
//}
//void FileSystem::deleteFile(const wchar_t* filePath)
//{
//	BOOL r = ::DeleteFileW(filePath);
//	if (r == FALSE) {
//		Win32IOErrorToExceptionThrow(::GetLastError(), filePath);
//	}
//}

//------------------------------------------------------------------------------
static void RemoveDirectoryImpl(LPCSTR lpPathName)
{
	BOOL r = ::RemoveDirectoryA(lpPathName);
	if (r == FALSE) {
		Win32IOErrorToExceptionThrow(::GetLastError(), lpPathName);
	}
}
static void RemoveDirectoryImpl(LPCWSTR lpPathName)
{
	BOOL r = ::RemoveDirectoryW(lpPathName);
	if (r == FALSE) {
		Win32IOErrorToExceptionThrow(::GetLastError(), lpPathName);
	}
}

//------------------------------------------------------------------------------
bool FileSystem::mkdir(const char* path)
{
	return ::CreateDirectoryA(path, NULL) != FALSE;
}
bool FileSystem::mkdir(const wchar_t* path)
{
	return ::CreateDirectoryW(path, NULL) != FALSE;
}
////------------------------------------------------------------------------------
//bool FileSystem::getAttributeInternal(const char* path, FileAttribute* outAttr)
//{
//	DWORD attr = ::GetFileAttributesA(path);
//	if (attr == -1) { return false; }
//
//	FileAttribute flags = FileAttribute::None;
//	if (attr & FILE_ATTRIBUTE_DIRECTORY)	flags |= FileAttribute::Directory;
//	else									flags |= FileAttribute::Normal;
//	if (attr & FILE_ATTRIBUTE_READONLY)		flags |= FileAttribute::ReadOnly;
//	if (attr & FILE_ATTRIBUTE_HIDDEN)		flags |= FileAttribute::Hidden;
//	*outAttr = flags;
//	return true;
//}
//bool FileSystem::getAttributeInternal(const wchar_t* path, FileAttribute* outAttr)
//{
//	DWORD attr = ::GetFileAttributesW(path);
//	if (attr == -1) { return false; }
//
//	FileAttribute flags = FileAttribute::None;
//	if (attr & FILE_ATTRIBUTE_DIRECTORY)	flags |= FileAttribute::Directory;
//	else									flags |= FileAttribute::Normal;
//	if (attr & FILE_ATTRIBUTE_READONLY)		flags |= FileAttribute::ReadOnly;
//	if (attr & FILE_ATTRIBUTE_HIDDEN)		flags |= FileAttribute::Hidden;
//	*outAttr = flags;
//	return true;
//}
//
//bool FileSystem::matchPath(const char* filePath, const char* pattern)
//{
//	return ::PathMatchSpecExA(filePath, pattern, PMSF_NORMAL) == S_OK;
//}
//
//bool FileSystem::matchPath(const wchar_t* filePath, const wchar_t* pattern)
//{
//	return ::PathMatchSpecExW(filePath, pattern, PMSF_NORMAL) == S_OK;
//}
//
LN_NAMESPACE_END
