#pragma once
#include "../Base/List.h"
#include "../Base/StringRef.h"
#include "PathName.h"

#if defined(LN_OS_FAMILY_UNIX)
#include <dirent.h>
#endif

LN_NAMESPACE_BEGIN
namespace detail { template<typename TChar> class GenericFileFinderImplBase; }

/**
	@brief	
*/
class DirectoryUtils
{
public:

	#pragma push_macro("GetCurrentDirectory")
	#undef GetCurrentDirectory
	/**
		@brief		カレントディレクトリのパスを取得します。
		@param[in]	outPath	: パス文字列を格納するバッファ (LN_MAX_PATH 文字数分のサイズが必要) 
		@return		パスの文字数 (\0 は除く)
	*/
	template<typename TChar>
	static size_t GetCurrentDirectory(TChar* outPath);

	template<typename TChar>
	static size_t LN_AFX_FUNCNAME(GetCurrentDirectory)(TChar* outPath);
	#pragma pop_macro("GetCurrentDirectory")

	/**
		@brief		ディレクトリ内に含まれる全てのファイルのパスを取得します。
		@param[in]	dirPath	: 検索するディレクトリ
		@param[in]	pattern	: 検索文字列 (ワイルドカード指定可能)
	*/
	static List<String> GetFiles(const TCHAR* dirPath, const TCHAR* pattern);


};


/**
	@brief	
*/
template<typename TChar>
class GenericFileFinder
{
public:
	GenericFileFinder(const GenericStringRef<TChar>& dirPath);
	~GenericFileFinder();
	bool IsWorking() const;
	const GenericPathName<TChar>& GetCurrent() const;
	bool Next();

private:
	detail::GenericFileFinderImplBase<TChar>*	m_impl;
};




namespace detail {

template<typename TChar>
class GenericFileFinderImplBase
{
public:
	virtual ~GenericFileFinderImplBase()
	{}

	bool IsWorking() const
	{
		return !m_combinedPath.IsEmpty();
	}

	const GenericPathName<TChar>& GetCurrent() const
	{
		return m_combinedPath;
	}

	virtual bool Next() = 0;

protected:
	GenericFileFinderImplBase(const GenericStringRef<TChar>& dirPath)
		: m_dirPath(dirPath)
	{}

	void SetCurrentFileName(const char* fileName)
	{
		if (fileName != nullptr)
			m_combinedPath.AssignUnderBasePath(m_dirPath, fileName);
		else
			m_combinedPath.Clear();
	}
	void SetCurrentFileName(const wchar_t* fileName)
	{
		if (fileName != nullptr)
			m_combinedPath.AssignUnderBasePath(m_dirPath, fileName);
		else
			m_combinedPath.Clear();
	}

protected:
	GenericPathName<TChar>	m_dirPath;
	GenericPathName<TChar>	m_combinedPath;
};

} // namespace detail


LN_NAMESPACE_END
