
#include <LuminoCore.h>
#include <Lumino/Xml/XmlWriter.h>
#include "../../../external/Lumino.Core/src/Fluorite/Frontend/Cpp/CppLexer.h"
#include "../../../external/Lumino.Core/src/Fluorite/DiagnosticsManager.h"
#include "../../../external/Lumino.Core/src/Fluorite/TokenList.h"
#include "../../../external/Lumino.Core/src/Fluorite/Combinators.h"
using namespace ln;

// 識別子、キーワード、演算子のみパースする
struct EFParserCursorCondition
{
	bool operator()(const ln::parser::Token& token) const
	{
		return
			token.GetCommonType() == parser::CommonTokenType::Identifier ||
			token.GetCommonType() == parser::CommonTokenType::Keyword ||
			token.GetCommonType() == parser::CommonTokenType::Operator ||
			token.GetCommonType() == parser::CommonTokenType::Eof;
	}
};
using EFParserCursor = combinators::GenericParserCursor<EFParserCursorCondition>;

enum class ClEntityType
{
	Class,
	Method,
	Property,
};

struct ClClass
{
	ClEntityType	type;
	String						name;
	Array<ln::parser::Token>	params;
};

struct EPTokenParser : public combinators::ParseLib<EFParserCursor>
{
	static ParserResult<ClClass> Parse_Method(ParserContext input)
	{
		LN_PARSE(r, TokenString("LNR_METHOD"));
		LN_PARSE(r2, TokenChar('('));
		LN_PARSE(r3, Until(TokenChar(')')));
		LN_PARSE(r4, Until(TokenChar('(')));
		return input.Success(ClClass{ ClEntityType::Method, r4.GetLast().ToString() });
	}

	static ParserResult<ClClass> Parse_Property(ParserContext input)
	{
		LN_PARSE(r, TokenString("LNR_PROPERTY"));
		LN_PARSE(r2, TokenChar('('));
		LN_PARSE(r3, Until(TokenChar(')')));
		LN_PARSE(r4, Until(TokenChar('(')));
		return input.Success(ClClass{ ClEntityType::Property, r4.GetLast().ToString() });
	}

	static ParserResult<ClClass> Parse_Class(ParserContext input)
	{
		LN_PARSE(v1, TokenString("LNR_CLASS"));
		LN_PARSE(v2, TokenChar('('));
		LN_PARSE(v3, Until(TokenChar(')')));
		LN_PARSE(v4, Until(TokenString("class")));
		LN_PARSE(r5, Token(ln::parser::CommonTokenType::Identifier));
		return input.Success(ClClass{ ClEntityType::Class, r5.ToString(), v3 });
	}

	static ParserResult<ClClass> Parse_LeadClass(ParserContext input)
	{
		LN_PARSE_RESULT(r1, SkipTill(Parser<ClClass>(Parse_Class) || Parser<ClClass>(Parse_Method) || Parser<ClClass>(Parse_Property)));	// 頭出し
		return input.Success(r1.GetValue());
	}

	static ParserResult<Array<ClClass>> Parse_AnalyzeUnit(ParserContext input)
	{
		LN_PARSE(r1, Many(Parser<ClClass>(Parse_LeadClass)));
		return input.Success(r1);
	}
};

int main(int argc, char **argv)
{
	if (argc != 3) return 1;

	//String text = FileSystem::ReadAllText(_T("D:/Proj/Lumino/include/Lumino/Graphics/Texture.h"));
	String text = FileSystem::ReadAllText(argv[1]);

	ln::parser::CppLexer lex;
	ln::parser::DiagnosticsItemSet diag;
	ln::parser::TokenListPtr tokenList = lex.Tokenize(text.c_str(), &diag);

	auto result = EPTokenParser::TryParse(EPTokenParser::Parser<Array<ClClass>>(EPTokenParser::Parse_AnalyzeUnit), tokenList);
	auto list = result.GetValue();

	//XmlFileWriter w(_T("out.xml"));
	XmlFileWriter w(argv[2]);
	w.WriteStartDocument();
	for (auto& entity : list)
	{
		TCHAR* typeTable[] =
		{
			_T("Class"),
			_T("Method"),
			_T("Property"),
		};

		w.WriteStartElement(_T("Entity"));
		w.WriteAttribute(_T("type"), typeTable[(int)entity.type]);
		w.WriteAttribute(_T("name"), entity.name);
		w.WriteEndElement();
	}
	w.WriteEndDocument();

	return 0;
}
