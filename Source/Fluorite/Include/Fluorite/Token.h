
#pragma once
#include <Lumino/Base/Collection.h>
#include "Common.h"

namespace fl {
class InputFile;
using SourceLocation = int32_t;

/**
	@brief	
*/
class SourceToken
{
public:
	SourceToken();
	SourceToken(const SourceToken& src) = default;
	SourceToken& operator = (const SourceToken& src) = default;
	~SourceToken();

	SourceToken(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd);
	SourceToken(InputFile* ownerFile, TokenGroup group, SourceLocation locBegin, SourceLocation locEnd, int tokenType);

	TokenGroup GetTokenGroup() const { return m_group; }

	/** トークンの種別。値の意味はプログラム言語ごとに異なる */
	int getTokenType() const { return m_tokenType; }

	int getLength() const { return m_locEnd - m_locBegin; }
	SourceLocation GetBeginLoc() const { return m_locBegin; }
	SourceLocation GetEndLoc() const { return m_locEnd; }

	const flChar* getBegin() const;
	const flChar* getEnd() const;

	int GetFirstLineNumber() const { return m_firstLineNumber; }
	int GetFirstColumn() const { return m_firstColumn; }
	int GetLastLineNumber() const { return m_lastLineNumber; }
	int GetLastColumn() const { return m_lastColumn; }

	const flChar* GetCStr(InputFile* file) const;	// not null terminator
	flString getString(InputFile* file) const;
	flString getString() const;


	/** 文字列が一致するか */
	bool EqualString(const char* str, int len = -1) const;

	/** 文字が一致するか */
	bool EqualChar(char ch) const;

	/** 文字列が一致するか */
	bool EqualGroupAndString(TokenGroup group, const char* str, int len = -1) const;


LN_INTERNAL_ACCESS:
	void SetFirstLineNumber(int lineNumber) { m_firstLineNumber = lineNumber; }
	void SetFirstColumn(int column) { m_firstColumn = column; }
	void SetLastLineNumber(int lineNumber) { m_lastLineNumber = lineNumber; }
	void SetLastColumn(int column) { m_lastColumn = column; }

private:
	InputFile*		m_ownerFile;		// コレが無いと文字列の取出しがものすごく面倒になる。効率より使いやすさ優先。

	// ポインタではなくオフセット値とし、シリアライズに備える
	SourceLocation	m_locBegin;			// トークンの開始位置
	SourceLocation	m_locEnd;			// トークンの終端位置 (最後の文字の次をさす)
	uint32_t		m_firstLineNumber;
	uint32_t		m_firstColumn;
	uint32_t		m_lastLineNumber;
	uint32_t		m_lastColumn;

	TokenGroup		m_group;
	int				m_tokenType;
};

/**
	@brief	
*/
class TokenList
	: public List<SourceToken>
{
public:
	TokenList() {}
	~TokenList() {}

	//String ToString(int begin, int end) const
	//{
	//	StringBuilder sb;
	//	for (int i = begin; i < end; ++i)
	//	{
	//		sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
	//	}
	//	return sb.ToString();
	//}

	//std::string toStringValidCode() const;

	template <class TPred>
	int indexOf(int startIndex, int count, TPred pred) const
	{
		for (int i = startIndex; i < startIndex + count && i < getCount(); i++)
		{
			if (pred(getAt(i))) return i;
		}
		return -1;
	}
};


class StringRef
{
public:
	typedef const char *iterator;
	static const size_t npos = ~size_t(0);

private:
	const char* m_str;
	size_t m_length;

	static size_t min(size_t a, size_t b) { return a < b ? a : b; }

public:
	StringRef()
		: m_str(0)
		, m_length(0)
	{}

	StringRef(const char* str)
		: m_str(str)
		, m_length(strlen(str))
	{}

	StringRef(const char* str, size_t length)
		: m_str(str)
		, m_length(length)
	{}

	StringRef(const char* begin, const char* end)
		: m_str(begin)
		, m_length(end - begin)
	{}

	StringRef(const std::string& str)
		: m_str(str.c_str())
		, m_length(str.length())
	{}

	iterator begin() const { return m_str; }

	iterator end() const { return m_str + m_length; }

	size_t length() const { return m_length; }

	bool startswith(StringRef prefix) const
	{
		return
			m_length >= prefix.m_length &&
			memcmp(m_str, prefix.m_str, prefix.m_length) == 0;
	}

	bool endswith(StringRef suffix) const
	{
		return
			m_length >= suffix.m_length &&
			memcmp(end() - suffix.m_length, suffix.m_str, suffix.m_length) == 0;
	}

	StringRef substr(size_t start, size_t n = npos) const
	{
		return StringRef(m_str + start, min(n, m_length - start));
	}

	const char& operator[](size_t index) const { return m_str[index]; }
};

/**
	@brief
*/
class Token
{
public:
	Token(SourceLocation loc);


	TokenGroup getTokenGroup() const { return (m_sourceToken) ? m_sourceToken->GetTokenGroup() : TokenGroup::Unknown; }

	StringRef strRef() const { return StringRef(m_sourceToken->getBegin(), m_sourceToken->getEnd()); }

	int getTokenType() const { return m_sourceToken->getTokenType(); }

	std::string getString() const { return m_sourceToken->getString(); }

	bool equalString(const char* str, int len = -1) const { return m_sourceToken->EqualString(str, len); }
	bool equalChar(char ch) const { return m_sourceToken->EqualChar(ch); }

	void setValid(bool valid) { m_valid = valid; }
	bool isValid() const { return m_valid; }

//private:
	SourceLocation	m_loc;
	SourceToken*	m_sourceToken;	// アクセス速度重視のため用意。元の List が変わると使えない
	bool			m_valid;
};





} // namespace fl
