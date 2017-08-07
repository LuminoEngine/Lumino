#pragma once
#include "../Base/List.h"
#include "../Base/StringRef.h"
#include "../Base/RefObject.h"
#include "PathName.h"

#if defined(LN_OS_FAMILY_UNIX)
#include <dirent.h>
#endif

LN_NAMESPACE_BEGIN
namespace detail { template<typename TChar> class GenericFileFinderImplBase; }

/**
	@brief	Obsolete
*/
class DirectoryUtils
{
public:

	/**
		@brief		カレントディレクトリのパスを取得します。
		@param[in]	outPath	: パス文字列を格納するバッファ (LN_MAX_PATH 文字数分のサイズが必要) 
		@return		パスの文字数 (\0 は除く)
	*/
	template<typename TChar>
	static size_t getCurrentDirectory(TChar* outPath);

	template<typename TChar>
	static size_t LN_AFX_FUNCNAME(getCurrentDirectory)(TChar* outPath);

	/**
		@brief		ディレクトリ内に含まれる全てのファイルのパスを取得します。
		@param[in]	dirPath	: 検索するディレクトリ
		@param[in]	pattern	: 検索文字列 (ワイルドカード指定可能)
	*/
	static List<String> getFiles(const TCHAR* dirPath, const TCHAR* pattern);


};


/**
	@brief	
*/
template<typename TChar>
class GenericFileFinder
	: public RefObject
{
public:
	GenericFileFinder(const GenericStringRef<TChar>& dirPath, const GenericStringRef<TChar>& pattern = GenericStringRef<TChar>());
	~GenericFileFinder();
	bool isWorking() const;
	const GenericPathName<TChar>& getCurrent() const;
	bool next();

private:
	detail::GenericFileFinderImplBase<TChar>*	m_impl;
	GenericString<TChar>	m_pattern;
};




namespace detail {

template<typename TChar>
class GenericFileFinderImplBase
{
public:
	virtual ~GenericFileFinderImplBase()
	{}

	bool isWorking() const
	{
		return !m_combinedPath.isEmpty();
	}

	const GenericPathName<TChar>& getCurrent() const
	{
		return m_combinedPath;
	}

	virtual bool next() = 0;

protected:
	GenericFileFinderImplBase(const GenericStringRef<TChar>& dirPath)
		: m_dirPath(dirPath)
	{}

	void setCurrentFileName(const char* fileName)
	{
		if (fileName != nullptr)
			m_combinedPath.assignUnderBasePath(m_dirPath, fileName);
		else
			m_combinedPath.clear();
	}
	void setCurrentFileName(const wchar_t* fileName)
	{
		if (fileName != nullptr)
			m_combinedPath.assignUnderBasePath(m_dirPath, fileName);
		else
			m_combinedPath.clear();
	}

protected:
	GenericPathName<TChar>	m_dirPath;
	GenericPathName<TChar>	m_combinedPath;
};

} // namespace detail


LN_NAMESPACE_END
