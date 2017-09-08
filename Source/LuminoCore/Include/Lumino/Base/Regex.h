
#pragma once
#include <regex>
#include "String.h"

LN_NAMESPACE_BEGIN

#ifdef LN_USTRING16
#else

class MatchResult;

/**
	@brief		
	@details	http://www.cplusplus.com/reference/regex/ECMAScript/
*/
class Regex
{
public:
	Regex(const StringRef& pattern);
	
	bool match(const StringRef& input, MatchResult* outResult = nullptr) const;
	
	bool search(const StringRef& input, MatchResult* outResult = nullptr) const;

	/** 
		@brief	対象の文字列が、正規表現パターンで表現できているかを調べます。（完全一致）
	*/
	static bool match(const StringRef& input, const StringRef& pattern, MatchResult* outResult = nullptr);

	/**
		@brief	文字列の中から、正規表現パターンに該当する文字列があるかを調べます。（検索, 部分一致）
	*/
	static bool search(const StringRef& input, const StringRef& pattern, MatchResult* outResult = nullptr);

private:
	std::basic_regex<Char>	m_regex;
};

/**
	@brief
*/
class MatchResult
{
public:

	/** マッチ範囲の先頭インデックスを返します。*/
	int getIndex() const { return static_cast<int>(m_matchResults.position()); }

	/** マッチ範囲の文字数を返します。*/
	int getLength() const { return static_cast<int>(m_matchResults.length()); }

	StringRef getValue() const
	{
		return getGroup(0);
	}
	
	int getGroupCount() const { return (int)m_matchResults.size(); }
	
	// index=0 はマッチした全体を返す
	StringRef getGroup(int index) const
	{
		if (LN_REQUIRE(0 <= index && index < (int)m_matchResults.size())) return StringRef();
		return StringRef(m_matchResults[index].first, m_matchResults[index].second);
	}
	
	StringRef operator[](int index) const
	{
		return getGroup(index);
	}

private:
	friend class Regex;

	typedef std::match_results<const Char*>	std_match_results;

	std_match_results	m_matchResults;
};



//==============================================================================
// Regex
//==============================================================================

inline Regex::Regex(const StringRef& pattern)
	: m_regex(pattern.getBegin(), pattern.getLength(), std::basic_regex<Char>::ECMAScript)
{
}

inline bool Regex::match(const StringRef& input, MatchResult* outResult) const
{
	if (outResult != nullptr)
	{
		return std::regex_match(input.getBegin(), input.getEnd(), outResult->m_matchResults, m_regex, std::regex_constants::match_default);
	}
	else
	{
		return std::regex_match(input.getBegin(), input.getEnd(), m_regex, std::regex_constants::match_default);
	}
}

inline bool Regex::search(const StringRef& input, MatchResult* outResult) const
{
	if (outResult != nullptr)
	{
		return std::regex_search(input.getBegin(), input.getEnd(), outResult->m_matchResults, m_regex, std::regex_constants::match_default);
	}
	else
	{
		return std::regex_search(input.getBegin(), input.getEnd(), m_regex, std::regex_constants::match_default);
	}
}

inline bool Regex::match(const StringRef& input, const StringRef& pattern, MatchResult* outResult)
{
	Regex re(pattern);
	return re.search(input, outResult);
}

inline bool Regex::search(const StringRef& input, const StringRef& pattern, MatchResult* outResult)
{
	Regex re(pattern);
	return re.search(input, outResult);
}

#endif
LN_NAMESPACE_END
