
#pragma once
#include "../Common.h"
#include "../TokenList.h"
#include "../TokenBuffer.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/**
	@brief
*/
class Lexer
{
public:
	struct Range
	{
		const TokenChar* pos;
		const TokenChar* end;

		Range()
		{
		}

		Range(const TokenChar* begin, const TokenChar* end_)
		{
			pos = begin;
			end = end_;
		}
	};

	struct ReadResult
	{
		void Set(const Token& t)
		{
			token = t;
		}

		Token	token;
		int		lineIncrementCount = 0;
		int		lastColumn = -1;
	};

public:
	ResultState Tokenize(UnitFile* file, DiagnosticsItemSet* diag);

	TokenListPtr Tokenize(const char* str, DiagnosticsItemSet* diag);	// TODO: ‚±‚ê‚¾‚Æo—ˆã‚ª‚Á‚½ Token ‚ªw‚µ‚Ä‚¢‚é•¶š—ñ‚ª‰ó‚ê‚Ä‚µ‚Ü‚¤
	TokenListPtr Tokenize(const ByteBuffer& buffer, DiagnosticsItemSet* diag);

	virtual int ReadToken(const Range& buffer, TokenList* list) = 0;
	virtual void PollingToken(const Token& newToken);

	static AlphaNumTypeFlags GetAlphaNumType(TokenChar ch);

	int ReadNewLine(const Range& buffer, ReadResult* outResult);
	static int IsNewLine(const Range& buffer);

	int ReadMBSSequence(const Range& buffer, ReadResult* outResult);

	
	//template<class TLexer>
	//static TokenListPtr TokenizeSimple(const StringRefA& str)
	//{
	//	TLexer lex;
	//	DiagnosticsItemSet diag;
	//	TokenListPtr tokens = lex.Tokenize(str.GetBegin(), &diag);	// TODO: ”ÍˆÍ
	//	LN_THROW(!diag.HasError(), InvalidFormatException);
	//	return tokens;
	//}

protected:
	virtual void OnStart();

	RefPtr<TokenBuffer>	m_tokenBuffer;
	DiagnosticsItemSet*	m_diag;
	int		m_currentLineNumber;	// 0`
	int		m_currentColumn;
};

} // namespace Parser
LN_NAMESPACE_END

