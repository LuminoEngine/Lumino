
#include "Parser.h"
#include <Lumino/Base/Enumerable.h>
using namespace fl;



HeaderParser::HeaderParser(SymbolDatabase* database)
	: m_database(database)
{
}

void HeaderParser::ParseFiles(const List<PathName>& pathes)
{
	for (auto& path : pathes)
	{
		ParseFile(path);
	}
}

void HeaderParser::ParseFile(const PathName& path)
{
	/*
	キーワードと { } ; に着目して切り分けるパーサ。
	※ LN_CLASS() の ( ) もパースできそうだが、ダメ。その外側の全ての ( ) も考慮する必要がある。それはもっと後段で。
	*/
	struct DeclsParser : public combinators::ParseLib<DeclsParser>
	{
		static ParserResult<Decl> Parse_EmptyDecl(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(';') || TokenChar(','));
			return input.Success(Decl{ _T("EmptyDecl"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_EmptyDecl2(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(';'));
			return input.Success(Decl{ _T("EmptyDecl"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LocalBlock(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar('{'));
			LN_PARSE_RESULT(r2, Many(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r3, TokenChar('}'));
			return input.Success(Decl{ _T("LocalBlock"), r1.GetMatchBegin(), r3.GetMatchEnd() });
		}
		
		static ParserResult<Decl> Parse_LN_ENUM(ParserContext input)	// ',' が他のパーサの邪魔をするので、ここでは　{～} だけを取り出す
		{
			LN_PARSE_RESULT(r1, TokenString("LN_ENUM_"));
			LN_PARSE_RESULT(r2, UntilMore(TokenChar('{')));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_EnumMember) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("enum"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}
		
		static ParserResult<Decl> Parse_EnumMember(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(','));
			return input.Success(Decl{ _T("enum_member"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_STRUCT(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_STRUCT"));
			LN_PARSE_RESULT(r2, UntilMore(TokenChar('{')));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FIELD) || Parser<Decl>(Parse_LN_METHOD) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("Struct"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}

		static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
			LN_PARSE_RESULT(r2, UntilMore(TokenChar('{')));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FIELD) || Parser<Decl>(Parse_LN_METHOD) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("class"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}

		static ParserResult<Decl> Parse_LN_FIELD(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_FIELD"));
			LN_PARSE_RESULT(r2, UntilMore(Parser<Decl>(Parse_EmptyDecl)));
			return input.Success(Decl{ _T("field"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_METHOD(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_METHOD"));
			LN_PARSE_RESULT(r2, UntilMore(Parser<Decl>(Parse_EmptyDecl2) || Parser<Decl>(Parse_LocalBlock)));
			return input.Success(Decl{ _T("method"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_DocumentComment(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Token(TokenGroup::Comment));
			return input.Success(Decl{ _T("document"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<List<Decl>> Parse_File(ParserContext input)
		{
			LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_ENUM) || Parser<Decl>(Parse_LN_CLASS) || Parser<Decl>(Parse_LN_STRUCT) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			return input.Success(r1);
		}

		static bool FilterToken(fl::Token* token)
		{
			if (token->GetTokenGroup() == TokenGroup::Comment &&
				StringTraits::Compare(token->GetBegin(), "/**", 3) == 0)
			{
				return true;
			}
			return
				token->EqualChar(';') || token->EqualChar(',') || token->EqualChar('{') || token->EqualChar('}') ||
				token->EqualString("LN_ENUM_", 8) ||
				token->EqualString("LN_STRUCT", 9) ||
				token->EqualString("LN_CLASS", 8) ||
				token->EqualString("LN_FIELD", 8) ||
				token->EqualString("LN_METHOD", 9) ||
				token->GetTokenGroup() == TokenGroup::Eof;	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
		}
	};


	auto codeStr = FileSystem::ReadAllText(path);
	StringA code = codeStr.ToStringA();

	fl::AnalyzerContext ctx;
	auto file = ctx.RegisterInputMemoryCode("memory", code.c_str());
	ctx.LexFile(file);
	auto tokens = file->GetTokenList();

	auto result = DeclsParser::TryParse(DeclsParser::Parse_File, tokens);
	for (auto decl1 : result.GetValue())
	{
		//Console::WriteLine(decl1.type);

		if (decl1.type == "document") ParseDocument(decl1);
		if (decl1.type == "Struct") ParseStructDecl(decl1);
		if (decl1.type == "class") ParseClassDecl(decl1);
		if (decl1.type == "enum") ParseEnumDecl(decl1);
		
		for (auto decl2 : decl1.decls)
		{
			//Console::WriteLine("  {0}", decl2.type);
		}
	}
}

HeaderParser::TokenItr HeaderParser::ParseMetadataDecl(TokenItr begin, TokenItr end)
{
	// (...) を探す
	auto paramBegin = std::find_if(begin, end, [](Token* t) { return t->EqualChar('('); }) + 1;
	auto paramEnd   = std::find_if(begin, end, [](Token* t) { return t->EqualChar(')'); });

	// (...) の中の 識別子,　英数字, 文字列, ',', '=' だけを取り出す
	auto argTokens = tr::MakeEnumerator::from(paramBegin, paramEnd + 1)
		.Where([](Token* t)
		{ 
			return
				t->GetTokenGroup() == TokenGroup::Identifier ||
				t->GetTokenGroup() == TokenGroup::ArithmeticLiteral ||
				t->GetTokenGroup() == TokenGroup::StringLiteral ||
				t->EqualChar(',') ||
				t->EqualChar('=');
		});

	auto info = std::make_shared<MetadataInfo>();

	String key, value;
	for (auto itr = argTokens.begin(); itr != argTokens.end();)
	{
		key = (*itr)->GetString();
		value = String::GetEmpty();
		++itr;
		if (itr != argTokens.end() && (*itr)->EqualChar('='))
		{
			++itr;
			value = (*itr)->GetString();
			++itr;
		}

		info->AddValue(key, value);
	}

	m_lastMetadata = info;
	return paramEnd + 1;
}

void HeaderParser::ParseStructDecl(const Decl& decl)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	auto name = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar('{'); }) - 2;

	auto info = std::make_shared<TypeInfo>();
	info->document = MoveLastDocument();
	info->name = (*name)->GetString();
	info->isStruct = true;
	m_database->structs.Add(info);

	for (auto& d : decl.decls)
	{
		if (d.type == "document") ParseDocument(d);
		if (d.type == "field") ParseFieldDecl(d, info);
		if (d.type == "method") ParseMethodDecl(d, info);
	}
}

void HeaderParser::ParseFieldDecl(const Decl& decl, TypeInfoPtr parent)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	auto name = decl.end - 2;
	auto type = decl.end - 4;

	auto info = std::make_shared<FieldInfo>();
	info->document = MoveLastDocument();
	info->name = (*name)->GetString();
	info->typeRawName = (*type)->GetString();
	parent->declaredFields.Add(info);
}

void HeaderParser::ParseMethodDecl(const Decl& decl, TypeInfoPtr parent)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);

	// param range
	auto lparen = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar('('); });
	auto rparen = std::find_if(lparen, decl.end, [](Token* t) { return t->EqualChar(')'); });

	// return type .. method name
	auto declTokens = tr::MakeEnumerator::from(paramEnd, lparen)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		//.Select([](Token* t) { return t->GetString(); })
		.ToList();

	//for (auto t : declTokens) Console::WriteLine(t->GetString());


	auto info = std::make_shared<MethodInfo>();
	info->owner = parent;
	info->metadata = MoveLastMetadata();
	info->document = MoveLastDocument();
	info->name = (declTokens.GetLast())->GetString();	// ( の直前を関数名として取り出す
	parent->declaredMethods.Add(info);

	// struct 型で戻り値が無ければコンストラクタ
	if (parent->isStruct && declTokens.begin() + 1 == declTokens.end())
		info->isConstructor = true;

	// return type
	if (info->isConstructor)
	{
		info->returnTypeRawName = "void";
	}
	else
	{
		auto begin = declTokens.begin();
		auto end = declTokens.end() - 1;
		int pointerLevel;
		ParseParamType(begin, end, &info->returnTypeRawName, &pointerLevel);

		for (auto itr = begin; itr != end; ++itr)
		{
			if ((*itr)->EqualString("static"))
				info->isStatic = true;
		}
	}

	if (!info->isStatic)
	{
		// class 型で InitializeXXXX ならコンストラクタ
		if (!parent->isStruct &&info->name.IndexOf("Initialize") == 0)
			info->isConstructor = true;
	}

	// mehod params
	auto paramBegin = lparen + 1;
	auto itr = paramBegin;
	for (; itr < rparen; ++itr)
	{
		if ((*itr)->EqualChar(','))
		{
			ParseParamsDecl(paramBegin, itr, info);
			paramBegin = itr + 1;
		}
	}
	if (paramBegin != rparen) ParseParamsDecl(paramBegin, rparen, info);

	// method attribute
	{
		auto begin = rparen + 1;
		auto end = std::find_if(begin, decl.end, [](Token* t) { return t->EqualChar('{'); });
		for (auto itr = begin; itr < end; ++itr)
		{
			if ((*itr)->EqualString("const"))
			{
				info->isConst = true;
			}
		}
	}
}

void HeaderParser::ParseParamsDecl(TokenItr begin, TokenItr end, MethodInfoPtr parent)
{
	auto declTokens = tr::MakeEnumerator::from(begin, end)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		.ToList();;
		//.Select([](Token* t) { return t->GetString(); })
		//

	//paramEnd = declTokens.end();
	auto paramEnd = std::find_if(declTokens.begin(), declTokens.end(), [](Token* t) { return t->EqualChar('='); });

	// , または =(デフォルト引数) の直前を引数名とする
	auto name = end - 1;

	StringA typeName;
	int pointerLevel;
	ParseParamType(declTokens.begin(), paramEnd - 1, &typeName, &pointerLevel);

	auto info = std::make_shared<ParameterInfo>();
	info->name = (*name)->GetString();
	info->typeRawName = typeName;
	parent->parameters.Add(info);
}

void HeaderParser::ParseParamType(TokenItr begin, TokenItr end, StringA* outName, int* outPointerLevel)
{
	auto type = begin;
	auto typeEnd = end;

	// lookup TypeName and count '*'
	TokenItr typeName;
	int pointerLevel = 0;
	for (auto itr = begin; itr != typeEnd; ++itr)
	{
		if ((*itr)->EqualChar('*')) pointerLevel++;
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier || (*itr)->GetTokenGroup() == TokenGroup::Keyword) typeName = itr;
	}

	*outName = (*typeName)->GetString();
	*outPointerLevel = pointerLevel;
}

void HeaderParser::ParseClassDecl(const Decl& decl)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	//auto name = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar(':'); }) - 2;
	TokenItr name;
	TokenItr itr = paramEnd;
	for (; itr < decl.end; ++itr)
	{
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier) name = itr;
		if ((*itr)->EqualChar(':') || (*itr)->EqualChar('{')) break;
	}

	auto info = std::make_shared<TypeInfo>();
	info->metadata = MoveLastMetadata();
	info->document = MoveLastDocument();
	info->name = (*name)->GetString();
	m_database->classes.Add(info);

	// base class name
	if ((*itr)->EqualChar(':'))
	{
		itr = std::find_if(itr, decl.end, [](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier; });
		info->baseClassRawName = (*itr)->GetString();
	}

	for (auto& d : decl.decls)
	{
		if (d.type == "document") ParseDocument(d);
		if (d.type == "method") ParseMethodDecl(d, info);
	}
}

void HeaderParser::ParseEnumDecl(const Decl& decl)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);

	// find name
	TokenItr name;
	TokenItr itr = paramEnd;
	for (; itr < decl.end; ++itr)
	{
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier) name = itr;
		if ((*itr)->EqualChar(':') || (*itr)->EqualChar('{')) break;
	}

	auto info = std::make_shared<TypeInfo>();
	info->document = MoveLastDocument();
	info->name = (*name)->GetString();
	info->isEnum = true;
	m_database->enums.Add(info);

	TokenItr last = itr;
	for (auto& d : decl.decls)
	{
		if (d.type == "document") ParseDocument(d);
		if (d.type == "enum_member")
		{
			ParseEnumMemberDecl(last, d.end, info);
			last = d.end;
		}
	}
}

void HeaderParser::ParseEnumMemberDecl(TokenItr begin, TokenItr end, TypeInfoPtr parent)
{
	// find name and value
	TokenItr name;
	TokenItr value;
	TokenItr itr = begin;
	for (; itr < end; ++itr)
	{
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier) name = itr;
		if ((*itr)->EqualChar(',') || (*itr)->EqualChar('=')) break;
	}
	if ((*itr)->EqualChar('='))
	{
		for (; itr < end; ++itr)
		{
			if ((*itr)->GetTokenGroup() == TokenGroup::ArithmeticLiteral) value = itr;
			if ((*itr)->EqualChar(',')) break;
		}
	}

	// create info
	auto info = std::make_shared<ConstantInfo>();
	info->document = MoveLastDocument();
	info->name = (*name)->GetString();
	info->value = (*value)->GetString().ToInt32();
	info->typeRawName = "int";
	parent->declaredConstants.Add(info);
}

void HeaderParser::ParseDocument(const Decl& decl)
{
	String doc = (*decl.begin)->GetString();

	// 改行コード統一し、コメント開始終了を削除する
	doc = doc
		.Replace("\r\n", "\n")
		.Replace("\r", "\n")
		.Replace("/**", "")
		.Replace("*/", "");

	auto info = std::make_shared<DocumentInfo>();

	List<String> lines = doc.Split("\n");
	String* target = &info->summary;
	for (String line : lines)
	{
		line = line.Trim();

		MatchResult result;
		if (Regex::Search(line, "@(\\w+)", &result))
		{
			if (result[1] == "brief")
			{
				target = &info->summary;
				line = line.Mid(result.GetLength());
			}
			else if (result[1] == "param")
			{
				String con = line.Mid(result.GetLength());
				if (Regex::Search(con, R"(\[(\w+)\]\s+(\w+)\s*\:\s*)", &result))
				{
					auto paramInfo = std::make_shared<ParameterDocumentInfo>();
					info->params.Add(paramInfo);
					paramInfo->io = result[1];
					paramInfo->name = result[2];
					target = &paramInfo->description;
					line = con.Mid(result.GetLength());
				}
			}
			else if (result[1] == "return")
			{
				target = &info->returns;
				line = line.Mid(result.GetLength());
			}
			else if (result[1] == "details")
			{
				target = &info->details;
				line = line.Mid(result.GetLength());
			}
		}

		(*target) += line.Trim();
	}

	m_lastDocument = info;
}

DocumentInfoPtr HeaderParser::MoveLastDocument()
{
	auto ptr = m_lastDocument;
	m_lastDocument = nullptr;
	return ptr;
}

MetadataInfoPtr HeaderParser::MoveLastMetadata()
{
	auto ptr = m_lastMetadata;
	m_lastMetadata = nullptr;
	return ptr;
}
