
#include <sys/types.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include "Internal.hpp"
#include <Windows.h>
#include <LuminoCore/Text/Encoding.hpp>
#include "PathHelper.hpp"
#include <LuminoCore/Base/Platform.hpp>

namespace ln {

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
        if (attr & FileAttribute::Directory) dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
        if (attr & FileAttribute::ReadOnly) dwAttr |= FILE_ATTRIBUTE_READONLY;
        if (attr & FileAttribute::Hidden) dwAttr |= FILE_ATTRIBUTE_HIDDEN;
        BOOL r = ::SetFileAttributesW(filePath, dwAttr);
        if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
    }

    static bool getAttribute(const wchar_t* path, FileAttribute* outAttr)
    {
        DWORD attr = ::GetFileAttributesW(path);
        if (attr == -1) {
            return false;
        }

        Flags<FileAttribute> flags = FileAttribute::None;
        if (attr & FILE_ATTRIBUTE_DIRECTORY)
            flags |= FileAttribute::Directory;
        else
            flags |= FileAttribute::Normal;
        if (attr & FILE_ATTRIBUTE_READONLY) flags |= FileAttribute::ReadOnly;
        if (attr & FILE_ATTRIBUTE_HIDDEN) flags |= FileAttribute::Hidden;
        *outAttr = flags;
        return true;
    }

	static void getLastModifiedTime(const wchar_t* path, time_t* outTime)
	{
		WIN32_FILE_ATTRIBUTE_DATA fad;
		BOOL r = ::GetFileAttributesExW(path, GetFileExInfoStandard, &fad);
		if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
		r = detail::Win32Helper::FILETIMEtoEpochTime(fad.ftLastWriteTime, outTime);
		if (LN_ENSURE(r != FALSE)) return;
	}

    static void copyFile(const wchar_t* sourceFileName, const wchar_t* destFileName, bool overwrite)
    {
        BOOL r = ::CopyFileW(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
        if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
    }

    static void removeFile(const wchar_t* filePath)
    {
        BOOL r = ::DeleteFileW(filePath);
        if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
    }

    static bool createDirectory(const wchar_t* path)
    {
        return ::CreateDirectoryW(path, NULL) != FALSE;
    }

    static void removeDirectory(const wchar_t* path)
    {
        BOOL r = ::RemoveDirectoryW(path);
        if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
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
    {
    }

    ~PlatformFileFinderImpl()
    {
        if (m_fh != INVALID_HANDLE_VALUE) {
            ::FindClose(m_fh);
        }
    }

    void initialize(const wchar_t* path, int len)
    {
        std::wstring pattern;
        pattern.reserve(len + 2);
        pattern.append(path, len);
        if (!detail::PathTraits::isSeparatorChar(pattern[len])) {
            pattern.append(L"\\");
        }
        pattern.append(L"*");

        m_fh = ::FindFirstFileW(pattern.c_str(), &m_fd);
        LN_ENSURE_IO(m_fh != INVALID_HANDLE_VALUE, path);
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
        do {
            if (::FindNextFileW(m_fh, &m_fd) != 0) {
                m_current = m_fd.cFileName;
            } else {
                m_current.clear();
                break;
            }

        } while (wcscmp(m_fd.cFileName, L".") == 0 || wcscmp(m_fd.cFileName, L"..") == 0);

        return !m_current.empty();
    }

private:
    HANDLE m_fh;
    WIN32_FIND_DATAW m_fd;
    std::wstring m_current;
};

static void RemoveDirectoryImpl(LPCSTR lpPathName)
{
    BOOL r = ::RemoveDirectoryA(lpPathName);
    if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
}
static void RemoveDirectoryImpl(LPCWSTR lpPathName)
{
    BOOL r = ::RemoveDirectoryW(lpPathName);
    if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
}

bool FileSystem::mkdir(const char* path)
{
    return ::CreateDirectoryA(path, NULL) != FALSE;
}
bool FileSystem::mkdir(const wchar_t* path)
{
    return ::CreateDirectoryW(path, NULL) != FALSE;
}

} // namespace ln
