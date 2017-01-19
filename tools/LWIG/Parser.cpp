
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

	flString key, value;
	for (auto itr = argTokens.begin(); itr != argTokens.end();)
	{
		key = (*itr)->GetString();
		value = flString::GetEmpty();
		++itr;
		if (itr != argTokens.end() && (*itr)->EqualChar('='))
		{
			++itr;
			value = (*itr)->GetString();
			++itr;
		}

		info->AddValue(String(key), String(value));
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
	info->name = String((*name)->GetString());
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
	info->name = String((*name)->GetString());
	info->typeRawName = String((*type)->GetString());
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
	info->name = String((declTokens.GetLast())->GetString());	// ( の直前を関数名として取り出す
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
		bool hasConst;
		ParseParamType(begin, end, &info->returnTypeRawName, &pointerLevel, &hasConst);

		for (auto itr = begin; itr != end; ++itr)
		{
			if ((*itr)->EqualString("static"))
				info->isStatic = true;
		}
	}

	if (!info->isStatic)
	{
		// class 型で InitializeXXXX ならコンストラクタ
		if (!parent->isStruct &&info->name.IndexOf(_T("Initialize")) == 0)
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

	String typeName;
	int pointerLevel;
	bool hasConst;
	ParseParamType(declTokens.begin(), paramEnd - 1, &typeName, &pointerLevel, &hasConst);

	auto info = std::make_shared<ParameterInfo>();
	info->name = String((*name)->GetString());
	info->typeRawName = typeName;
	info->isOut = (!hasConst && pointerLevel > 0);
	parent->parameters.Add(info);
}

void HeaderParser::ParseParamType(TokenItr begin, TokenItr end, String* outName, int* outPointerLevel, bool* outHasConst)
{
	auto type = begin;
	auto typeEnd = end;

	// lookup TypeName and count '*'
	TokenItr typeName;
	int pointerLevel = 0;
	bool hasConst = false;
	for (auto itr = begin; itr != typeEnd; ++itr)
	{
		if ((*itr)->EqualChar('*')) pointerLevel++;
		if ((*itr)->EqualString("const")) hasConst = true;
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier || (*itr)->GetTokenGroup() == TokenGroup::Keyword) typeName = itr;
	}

	*outName = String((*typeName)->GetString());
	*outPointerLevel = pointerLevel;
	*outHasConst = hasConst;
}

void HeaderParser::ParseClassDecl(const Decl& decl)
{
	/*
	LN_CLASS()
	template<typename T>
	class TestList
	{
	};

	LN_CLASS()
	class TestList2
		: public TestList<Engine>
	{
	};
	*/
	struct ClassHeaderParser : public combinators::ParseLib<ClassHeaderParser>
	{
		struct TypeName
		{
			flString		name;
			List<flString>	typeArgs;
		};

		struct ClassHeader
		{
			List<flString>	templateTypeParams;
			flString		className;
			List<TypeName>	baseClassNames;
		};

		//----------------------------------------------------------------------
		// template <...>
		static ParserResult<List<flString>> Parse_TemplateDecl(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("template"));
			LN_PARSE_RESULT(r2, TokenChar('<'));
			LN_PARSE_RESULT(r3, Parse_TemplateParamList);
			LN_PARSE_RESULT(r4, TokenChar('>'));
			return input.Success(r3.GetValue());
		}

		static ParserResult<List<flString>> Parse_TemplateParamList(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Parse_TemplateParam);
			LN_PARSE_RESULT(r2, Many(Parser<flString>(Parse_TemplateParamLater)));
			List<flString> list{ r1.GetValue() };
			list.AddRange(r2.GetValue());
			return input.Success(list);
		}

		static ParserResult<flString> Parse_TemplateParam(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("typename") || TokenString("class"));
			LN_PARSE_RESULT(r2, Token(TokenGroup::Identifier));
			return input.Success(r2.GetValue()->GetString());
		}

		static ParserResult<flString> Parse_TemplateParamLater(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(','));
			LN_PARSE_RESULT(r2, TokenString("typename") || TokenString("class"));
			LN_PARSE_RESULT(r3, Token(TokenGroup::Identifier));
			return input.Success(r3.GetValue()->GetString());
		}

		//----------------------------------------------------------------------
		// class
		static ParserResult<flString> Parse_ClassMain(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("class"));
			LN_PARSE_RESULT(r2, Token(TokenGroup::Identifier));
			return input.Success(r2.GetValue()->GetString());
		}

		//----------------------------------------------------------------------
		// base classes:
		static ParserResult<List<TypeName>> Parse_BaseClasses(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(':'));
			LN_PARSE_RESULT(r2, Many(Parser<TypeName>(Parse_BaseTypeName)));
			return input.Success(r2.GetValue());
		}
		
		static ParserResult<TypeName> Parse_BaseTypeName(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("public"));
			LN_PARSE_RESULT(r2, Token(TokenGroup::Identifier));
			LN_PARSE_RESULT(r3, Optional(Parser<List<flString>>(Parse_TypeParamList)));
			return input.Success(TypeName{ r2.GetValue()->GetString(), r3.GetValue().GetValue() });
		}

		static ParserResult<List<flString>> Parse_TypeParamList(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar('<'));
			LN_PARSE_RESULT(r2, Parse_TypeParam);
			LN_PARSE_RESULT(r3, Many(Parser<flString>(Parse_TypeParamLater)));
			LN_PARSE_RESULT(r4, TokenChar('>'));
			List<flString> list{ r2.GetValue() };
			list.AddRange(r3.GetValue());
			return input.Success(list);
		}

		static ParserResult<flString> Parse_TypeParam(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Token(TokenGroup::Identifier));
			return input.Success(r1.GetValue()->GetString());
		}

		static ParserResult<flString> Parse_TypeParamLater(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(','));
			LN_PARSE_RESULT(r2, Token(TokenGroup::Identifier));
			return input.Success(r2.GetValue()->GetString());
		}

		//----------------------------------------------------------------------
		// class header:
		static ParserResult<ClassHeader> Parse_ClassHeader(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Optional(Parser<List<flString>>(Parse_TemplateDecl)));
			LN_PARSE_RESULT(r2, Parse_ClassMain);
			LN_PARSE_RESULT(r3, Optional(Parser<List<TypeName>>(Parse_BaseClasses)));
			LN_PARSE_RESULT(r4, TokenChar('{'));
			return input.Success(ClassHeader{ r1.GetValue().GetValue(), r2.GetValue(), r3.GetValue().GetValue(), });
		}

		static bool FilterToken(fl::Token* token)
		{
			return
				token->EqualChar('<') ||
				token->EqualChar('>') ||
				token->EqualChar(',') ||
				token->EqualChar(':') ||
				token->EqualChar('{') ||
				token->EqualGroupAndString(TokenGroup::Keyword, "template") ||
				token->EqualGroupAndString(TokenGroup::Keyword, "typename") ||
				token->EqualGroupAndString(TokenGroup::Keyword, "class") ||
				token->EqualGroupAndString(TokenGroup::Keyword, "public") ||
				token->GetTokenGroup() == TokenGroup::Identifier ||
				token->GetTokenGroup() == TokenGroup::Eof;	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
		}
	};

	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);

	auto& result = ClassHeaderParser::TryParse(ClassHeaderParser::Parse_ClassHeader, paramEnd, decl.end);
	auto& classHeader = result.GetValue();


	//auto name = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar(':'); }) - 2;
	//TokenItr name;
	//TokenItr itr = paramEnd;
	//for (; itr < decl.end; ++itr)
	//{
	//	if ((*itr)->GetTokenGroup() == TokenGroup::Identifier) name = itr;
	//	if ((*itr)->EqualChar(':') || (*itr)->EqualChar('{')) break;
	//}

	auto info = std::make_shared<TypeInfo>();
	info->metadata = MoveLastMetadata();
	info->document = MoveLastDocument();
	info->name = String(classHeader.className);
	if (!classHeader.baseClassNames.IsEmpty()) info->baseClassRawName = String(classHeader.baseClassNames[0].name);
	m_database->classes.Add(info);

	// base class name
	//if ((*itr)->EqualChar(':'))
	//{
	//	itr = std::find_if(itr, decl.end, [](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier; });
	//	info->baseClassRawName = (*itr)->GetString();
	//}

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
	info->name = String((*name)->GetString());
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
	info->name = String((*name)->GetString());
	info->value = (*value)->GetString().ToInt32();
	info->typeRawName = "int";
	parent->declaredConstants.Add(info);
}

void HeaderParser::ParseDocument(const Decl& decl)
{
	String doc((*decl.begin)->GetString());

	// 改行コード統一し、コメント開始終了を削除する
	doc = doc
		.Replace(_T("\r\n"), _T("\n"))
		.Replace(_T("\r"), _T("\n"))
		.Replace(_T("/**"), _T(""))
		.Replace(_T("*/"), _T(""));

	auto info = std::make_shared<DocumentInfo>();

	List<String> lines = doc.Split(_T("\n"));
	String* target = &info->summary;
	for (String line : lines)
	{
		line = line.Trim();

		MatchResult result;
		if (Regex::Search(line, _T("@(\\w+)"), &result))
		{
			if (result[1] == _T("brief"))
			{
				target = &info->summary;
				line = line.Mid(result.GetLength());
			}
			else if (result[1] == _T("param"))
			{
				String con = line.Mid(result.GetLength());
				if (Regex::Search(con, _T(R"(\[(\w+)\]\s+(\w+)\s*\:\s*)"), &result))
				{
					auto paramInfo = std::make_shared<ParameterDocumentInfo>();
					info->params.Add(paramInfo);
					paramInfo->io = result[1];
					paramInfo->name = result[2];
					target = &paramInfo->description;
					line = con.Mid(result.GetLength());
				}
			}
			else if (result[1] == _T("return"))
			{
				target = &info->returns;
				line = line.Mid(result.GetLength());
			}
			else if (result[1] == _T("details"))
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
