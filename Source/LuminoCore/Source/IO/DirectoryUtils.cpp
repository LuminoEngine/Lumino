
#include "../Internal.h"
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/DirectoryUtils.h>
#include <Lumino/IO/PathTraits.h>
#if defined(LN_OS_WIN32)
#include "FileFinder_Win32.h"
#else
#include "FileFinder_UNIX.h"
#endif

LN_NAMESPACE_BEGIN
	
//==============================================================================
// DirectoryUtils
//==============================================================================

//------------------------------------------------------------------------------
template<typename TChar>
static size_t GetCurrentDirectoryInternal(TChar* outPath);

template<>
static size_t GetCurrentDirectoryInternal(char* outPath)
{
#ifdef LN_OS_WIN32
	return ::GetCurrentDirectoryA(LN_MAX_PATH, outPath);
#else
	return strlen(getcwd(outPath, LN_MAX_PATH));
#endif
}
template<>
static size_t GetCurrentDirectoryInternal(wchar_t* outPath)
{
#ifdef LN_OS_WIN32
	return ::GetCurrentDirectoryW(LN_MAX_PATH, outPath);
#else
	LN_THROW(0, NotImplementedException);
#endif
}

#pragma push_macro("GetCurrentDirectory")
#undef GetCurrentDirectory
template<typename TChar>
size_t DirectoryUtils::GetCurrentDirectory(TChar* outPath) { return LN_AFX_FUNCNAME(GetCurrentDirectory)(outPath); }
template<typename TChar>
size_t DirectoryUtils::LN_AFX_FUNCNAME(GetCurrentDirectory)(TChar* outPath)
{
	return GetCurrentDirectoryInternal(outPath);
}
template size_t DirectoryUtils::GetCurrentDirectory<char>(char* outPath);
template size_t DirectoryUtils::LN_AFX_FUNCNAME(GetCurrentDirectory)<char>(char* outPath);
template size_t DirectoryUtils::GetCurrentDirectory<wchar_t>(wchar_t* outPath);
template size_t DirectoryUtils::LN_AFX_FUNCNAME(GetCurrentDirectory)<wchar_t>(wchar_t* outPath);
#pragma pop_macro("GetCurrentDirectory")

//------------------------------------------------------------------------------
#ifdef _WIN32
List<String> DirectoryUtils::GetFiles(const TCHAR* drPath, const TCHAR* pattern)
{
	List<String> fileList;
	PathName dirPathKey(drPath);
	String dirPatternPath(dirPathKey.GetStrEndSeparator());
	if (pattern) {
		dirPathKey.Append(pattern);
	}
	else {
		dirPathKey.Append(_T("*"));
	}

    // 検索開始
	WIN32_FIND_DATA fd;
	HANDLE h = ::FindFirstFile(dirPathKey.c_str(), &fd);
	if (h == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = ::GetLastError();
		if (dwError == ERROR_FILE_NOT_FOUND ||
			dwError == ERROR_NO_MORE_FILES){
			// これらは許可。空の配列を返す。
			return fileList;
		}
		else {
			LN_THROW(0, Win32Exception, dwError);
		}
	}

    do
	{
        if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            // ディレクトリの場合
        }
        else {
            // ファイルの場合
			fileList.Add(dirPatternPath + fd.cFileName);
        }

		// 次のファイルを検索する
	} while (::FindNextFile(h, &fd));

	// 終了
	::FindClose(h);
	return fileList;
}

#else
List<String> DirectoryUtils::GetFiles(const TCHAR* drPath, const TCHAR* pattern)
{
	LN_THROW(0, NotImplementedException);
	// http://www.syuhitu.org/other/dir.html
	List<String> fileList;
	return fileList;
}
#endif



//==============================================================================
// GenericFileFinder
//==============================================================================


//------------------------------------------------------------------------------
template<typename TChar>
GenericFileFinder<TChar>::GenericFileFinder(const GenericStringRef<TChar>& dirPath)
	: m_impl(LN_NEW detail::GenericFileFinderImpl<TChar>(dirPath))
{
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericFileFinder<TChar>::~GenericFileFinder()
{
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericFileFinder<TChar>::IsWorking() const
{
	return m_impl->IsWorking();
}

//------------------------------------------------------------------------------
template<typename TChar>
const GenericPathName<TChar>& GenericFileFinder<TChar>::GetCurrent() const
{
	return m_impl->GetCurrent();
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericFileFinder<TChar>::Next()
{
	return m_impl->Next();
}

// テンプレートのインスタンス化
template class GenericFileFinder<char>;
template class GenericFileFinder<wchar_t>;

LN_NAMESPACE_END
