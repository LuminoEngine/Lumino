#include "TestConfig.h"
#include <Fluorite/ParserCombinators.h>

class Test_ParserCombinators : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_ParserCombinators, parse)
{
	struct TokenParser : public combinators::ParseLib<TokenParser>
	{
		struct Decl
		{
			String		type;
			combinators::Iterator			begin;
			combinators::Iterator			end;
			List<Decl>	decls;
		};

		static ParserResult<Decl> Parse_EmptyDecl(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(';'));
			return input.Success(Decl{ _T("EmptyDecl"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_METHOD(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_METHOD"));
			LN_PARSE_RESULT(r2, UntilMore(TokenChar(';')));
			return input.Success(Decl{ _T("LN_METHOD"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
			LN_PARSE_RESULT(r2, TokenChar('{'));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LN_METHOD)));	// ネスト	TODO: できれば <Decl> はやめたい
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("LN_CLASS"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.getValue() });
		}

		static ParserResult<List<Decl>> Parse_File(ParserContext input)
		{
			LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_CLASS)));
			return input.Success(r1);
		}

		static bool FilterToken(fl::Token* token)
		{
			return
				token->EqualChar(';') || token->EqualChar('{') || token->EqualChar('}') ||
				token->EqualString("LN_CLASS", 8) ||
				token->EqualString("LN_METHOD", 9) ||
				token->GetTokenGroup() == TokenGroup::Eof;	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
		}
	};

	DO_LEX("LN_CLASS class A { int a; LN_METHOD int Func(); };");

	auto result = TokenParser::TryParse(TokenParser::Parse_File, tokens);

	ASSERT_EQ(true, result.IsSucceed());
	auto d = result.getValue()[0];
	ASSERT_EQ(2, d.decls.getCount());
	ASSERT_EQ(_T("EmptyDecl"), d.decls[0].type);
	ASSERT_EQ(_T("LN_METHOD"), d.decls[1].type);
}

//-----------------------------------------------------------------------------
TEST_F(Test_ParserCombinators, AcceptsThatChar)
{
	struct TokenParser : public combinators::ParseLib<TokenParser>
	{
		// 戻り値が ParserResult<>、引数が ParserContext である関数は Parser 関数。
		// LN_PARSE() はこの中に書くことができる。
		static ParserResult<int> Parse_String(ParserContext input)
		{
			LN_PARSE(r1, TokenChar('a'));
			return input.Success(100);
		}
	};

	{
		DO_LEX("a");
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(true, result.IsSucceed());
		ASSERT_EQ(100, result.getValue());
		ASSERT_EQ(0, result.GetMatchBegin() - tokens->begin());
		ASSERT_EQ(1, result.GetMatchEnd() - tokens->begin());
	}
	{
		DO_LEX("a a");	// ※ "aa" だと 1 つの Token になるので分割されるようにする
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(true, result.IsSucceed());
		ASSERT_EQ(100, result.getValue());
		ASSERT_EQ(0, result.GetMatchBegin() - tokens->begin());
		ASSERT_EQ(1, result.GetMatchEnd() - tokens->begin());		// 1つ目のトークンまでマッチ成功
	}
	// <Test> 不一致
	{
		DO_LEX("b");
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(false, result.IsSucceed());
		ASSERT_EQ(true, result.getMessage().indexOf("Unexpected") >= 0);	// なんかエラーメッセージが入っているはず
	}
	// <Test> 空文字列の入力
	{
		DO_LEX("");
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(false, result.IsSucceed());
	}
}
