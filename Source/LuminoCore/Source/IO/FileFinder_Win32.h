
#include "../Internal.h"
#include <windows.h>

LN_NAMESPACE_BEGIN
namespace detail {

template<typename TChar>
class GenericFileFinderImpl
{
};

//------------------------------------------------------------------------------
template<typename TChar>
static inline void MakePattern(const GenericStringRef<TChar>& path, TChar* pattern)
{
	int len = path.CopyTo(pattern, LN_MAX_PATH);
	if (!PathTraits::IsSeparatorChar(pattern[len]))
	{
		LN_THROW(len < LN_MAX_PATH - 1, ArgumentException);
		pattern[len] = '/';
		len++;
	}
	LN_THROW(len < LN_MAX_PATH - 1, ArgumentException);
	pattern[len] = '*';
	len++;
	pattern[len] = '\0';
}

//------------------------------------------------------------------------------
// for char
template<>
class GenericFileFinderImpl<char>
	: public GenericFileFinderImplBase<char>
{
public:
	GenericFileFinderImpl(const GenericStringRef<char>& dirPath)
		: GenericFileFinderImplBase<char>(dirPath)
		, m_fh(INVALID_HANDLE_VALUE)
	{
		char pattern[LN_MAX_PATH];
		MakePattern(dirPath, pattern);

		m_fh = ::FindFirstFileA(pattern, &m_fd);
		if (m_fh == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = ::GetLastError();
			if (dwError == ERROR_FILE_NOT_FOUND ||
				dwError == ERROR_NO_MORE_FILES)
			{
				SetCurrentFileName((char*)NULL);
			}
			else
			{
				LN_THROW(0, Win32Exception, dwError);
			}
		}
		else
		{
			SetCurrentFileName(m_fd.cFileName);
			if (strcmp(m_fd.cFileName, ".") == 0 || strcmp(m_fd.cFileName, "..") == 0)
			{
				Next();
			}
		}
	}

	~GenericFileFinderImpl()
	{
		if (m_fh != INVALID_HANDLE_VALUE)
		{
			::FindClose(m_fh);
		}
	}

	virtual bool Next() override
	{
		do
		{
			if (::FindNextFileA(m_fh, &m_fd) != 0)
			{
				SetCurrentFileName(m_fd.cFileName);
			}
			else
			{
				m_fd.cFileName[0] = '\0';
				SetCurrentFileName((char*)NULL);
			}
		} while (strcmp(m_fd.cFileName, ".") == 0 || strcmp(m_fd.cFileName, "..") == 0);

		return !GetCurrent().IsEmpty();
	}

private:
	HANDLE				m_fh;
	WIN32_FIND_DATAA	m_fd;
};

//------------------------------------------------------------------------------
// for wchar_t
template<>
class GenericFileFinderImpl<wchar_t>
	: public GenericFileFinderImplBase<wchar_t>
{
public:
	GenericFileFinderImpl(const GenericStringRef<wchar_t>& dirPath)
		: GenericFileFinderImplBase<wchar_t>(dirPath)
		, m_fh(INVALID_HANDLE_VALUE)
	{
		wchar_t pattern[LN_MAX_PATH];
		MakePattern(dirPath, pattern);

		m_fh = ::FindFirstFileW(pattern, &m_fd);
		if (m_fh == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = ::GetLastError();
			if (dwError == ERROR_FILE_NOT_FOUND ||
				dwError == ERROR_NO_MORE_FILES)
			{
				SetCurrentFileName((wchar_t*)NULL);
			}
			else
			{
				LN_THROW(0, Win32Exception, dwError);
			}
		}
		else
		{
			SetCurrentFileName(m_fd.cFileName);
			if (wcscmp(m_fd.cFileName, L".") == 0 || wcscmp(m_fd.cFileName, L"..") == 0)
			{
				Next();
			}
		}
	}

	~GenericFileFinderImpl()
	{
		if (m_fh != INVALID_HANDLE_VALUE)
		{
			::FindClose(m_fh);
		}
	}

	virtual bool Next() override
	{
		do
		{
			if (::FindNextFileW(m_fh, &m_fd) != 0)
			{
				SetCurrentFileName(m_fd.cFileName);
			}
			else
			{
				m_fd.cFileName[0] = '\0';
				SetCurrentFileName((wchar_t*)NULL);
			}
		} while (wcscmp(m_fd.cFileName, L".") == 0 || wcscmp(m_fd.cFileName, L"..") == 0);

		return !GetCurrent().IsEmpty();
	}

private:
	HANDLE				m_fh;
	WIN32_FIND_DATAW	m_fd;
};

} // namespace detail
LN_NAMESPACE_END
