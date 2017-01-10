
#include "Parser.h"
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/ParserCombinators.h>
using namespace fl;

struct DeclsParser : public combinators::ParseLib<DeclsParser>
{
	struct Decl
	{
		String		type;
		int			begin;
		int			end;
		List<Decl>	decls;
	};

	static ParserResult<Decl> Parse_EmptyDecl(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenChar(';'));
		return input.Success(Decl{ _T("EmptyDecl"), r1.GetMatchBegin(), r1.GetMatchEnd() });
	}

	static ParserResult<Decl> Parse_LN_METHOD(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenString("LN_FUNCTION"));
		LN_PARSE_RESULT(r2, UntilMore(TokenChar(';')));
		return input.Success(Decl{ _T("LN_FUNCTION"), r1.GetMatchBegin(), r2.GetMatchEnd() });
	}

	static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
		LN_PARSE_RESULT(r2, TokenChar('{'));
		LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_METHOD) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_DocumentComment)));
		LN_PARSE_RESULT(r4, TokenChar('}'));
		return input.Success(Decl{ _T("LN_CLASS"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
	}

	static ParserResult<Decl> Parse_DocumentComment(ParserContext input)
	{
		LN_PARSE_RESULT(r1, Token(TokenGroup::Comment));
		return input.Success(Decl{ _T("DocumentComment"), r1.GetMatchBegin(), r1.GetMatchEnd() });
	}

	static ParserResult<List<Decl>> Parse_File(ParserContext input)
	{
		LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_CLASS) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_DocumentComment)));
		return input.Success(r1);
	}

	static bool FilterToken(const fl::Token& token)
	{
		if (token.GetTokenGroup() == TokenGroup::Comment &&
			StringTraits::Compare(token.GetBegin(), "/**", 3) == 0)
		{
			return true;
		}
		return
			token.EqualChar(';') || token.EqualChar('{') || token.EqualChar('}') ||
			token.EqualString("LN_CLASS", 8) ||
			token.EqualString("LN_FUNCTION", 11) ||
			token.GetTokenGroup() == TokenGroup::Eof;	// TODO: Ç±ÇÍÇ™ñ≥Ç≠ÇƒÇ‡Ç¢Ç¢ÇÊÇ§Ç…ÇµÇΩÇ¢ÅBç°ÇÕÇ±ÇÍÇ™Ç»Ç¢Ç∆ÅAManyíÜÇ…EOFÇµÇΩÇ∆Ç´OutOfRangeÇ∑ÇÈ
	}
};


void HeaderParser::ParseFiles(const List<PathName>& pathes)
{
	for (auto& path : pathes)
	{
		ParseFile(path);
	}
}

void HeaderParser::ParseFile(const PathName& path)
{
	auto codeStr = FileSystem::ReadAllText(path);
	StringA code = codeStr.ToStringA();

	fl::AnalyzerContext ctx;
	auto file = ctx.RegisterInputMemoryCode("memory", code.c_str());
	ctx.LexFile(file);
	auto tokens = file->GetTokenList();

	auto result = DeclsParser::TryParse(DeclsParser::Parse_File, tokens);
	for (auto decl1 : result.GetValue())
	{
		Console::WriteLine(decl1.type);

		for (auto decl2 : decl1.decls)
		{
			Console::WriteLine("  {0}", decl2.type);
		}
	}
}

