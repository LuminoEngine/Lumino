
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
			LN_PARSE_RESULT(r2, Many(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment) || Parser<Decl>(Parse_AccessLevel)));
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
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FIELD) || Parser<Decl>(Parse_LN_METHOD) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment) || Parser<Decl>(Parse_AccessLevel)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("Struct"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}

		static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
			LN_PARSE_RESULT(r2, UntilMore(TokenChar('{')));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FIELD) || Parser<Decl>(Parse_LN_METHOD) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment) || Parser<Decl>(Parse_AccessLevel)));
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

		static ParserResult<Decl> Parse_LN_DELEGATE(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_DELEGATE"));
			LN_PARSE_RESULT(r2, UntilMore(Parser<Decl>(Parse_EmptyDecl2)));
			return input.Success(Decl{ _T("delegate"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_DocumentComment(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Token(TokenGroup::Comment));
			return input.Success(Decl{ _T("document"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_AccessLevel(ParserContext input)
		{
			//LN_PARSE_RESULT(r1, TokenString("public") || TokenString("protected") || TokenString("private") || TokenString("LN_INTERNAL_ACCESS"));
			LN_PARSE_RESULT(r2, TokenChar(':'));
			return input.Success(Decl{ _T("AccessLevelLike"), r2.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<List<Decl>> Parse_File(ParserContext input)
		{
			LN_PARSE(r1, Many(
				Parser<Decl>(Parse_LN_ENUM) ||
				Parser<Decl>(Parse_LN_CLASS) ||
				Parser<Decl>(Parse_LN_STRUCT) ||
				Parser<Decl>(Parse_LN_DELEGATE) ||
				Parser<Decl>(Parse_EmptyDecl2) ||
				Parser<Decl>(Parse_LocalBlock) ||
				Parser<Decl>(Parse_DocumentComment) ||
				Parser<Decl>(Parse_AccessLevel)));
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
				token->EqualString("LN_DELEGATE", 11) ||
				token->EqualChar(':') ||// token->EqualString("public", 6) || token->EqualString("protected", 9) || token->EqualString("private", 6) || token->EqualString("LN_INTERNAL_ACCESS", 18) ||
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
		if (decl1.type == "document") ParseDocument(decl1);
		if (decl1.type == "Struct") ParseStructDecl(decl1);
		if (decl1.type == "class") ParseClassDecl(decl1);
		if (decl1.type == "enum") ParseEnumDecl(decl1);
		if (decl1.type == "delegate") ParseDelegateDecl(decl1);
		
		for (auto decl2 : decl1.decls)
		{
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

	m_currentAccessLevel = AccessLevel::Public;
	for (auto& d : decl.decls)
	{
		if (d.type == "AccessLevelLike") ParseAccessLevel(d);
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
	TokenItr rparen;
	{
		int level = 1;
		for (auto itr = lparen + 1; itr != decl.end; ++itr)
		{
			if ((*itr)->EqualChar('(')) level++;
			if ((*itr)->EqualChar(')')) level--;
			if (level == 0)
			{
				rparen = itr;
				break;
			}
		}
	}

	// return type .. method name
	auto declTokens = tr::MakeEnumerator::from(paramEnd, lparen)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		.ToList();

	auto info = std::make_shared<MethodInfo>();
	info->owner = parent;
	info->metadata = MoveLastMetadata();
	info->document = MoveLastDocument();
	info->accessLevel = m_currentAccessLevel;
	info->name = String((declTokens.GetLast())->GetString());	// ( の直前を関数名として取り出す

	if (info->metadata->HasKey("Document"))
		parent->declaredMethodsForDocument.Add(info);	// ドキュメント抽出用メソッド
	else
		parent->declaredMethods.Add(info);				// 普通にラッパーを作るメソッド

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
		bool hasVirtual;
		ParseParamType(begin, end, &info->returnTypeRawName, &pointerLevel, &hasConst, &hasVirtual);

		for (auto itr = begin; itr != end; ++itr)
		{
			if ((*itr)->EqualString("static"))
				info->isStatic = true;
		}
		info->isVirtual = hasVirtual;
	}

	if (!info->isStatic)
	{
		// class 型で InitializeXXXX ならコンストラクタ
		if (!parent->isStruct &&info->name.IndexOf(_T("Initialize")) == 0)
			info->isConstructor = true;
	}

	// mehod params
	ParseParamListDecl(lparen, rparen, info);

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

//------------------------------------------------------------------------------
//	begin	=	'('
//	end		=	')'
void HeaderParser::ParseParamListDecl(TokenItr begin, TokenItr end, MethodInfoPtr parent)
{
	auto paramBegin = begin + 1;
	auto itr = paramBegin;
	//bool isDelegate = false;
	for (; itr < end; ++itr)
	{
		//if ((*itr)->EqualString(_T("Delegate")))
		//{
		//	isDelegate = true;	// 今解析中の引数は Delegate である
		//}

		if ((*itr)->EqualChar(','))
		{
			ParseParamDecl(paramBegin, itr, parent, false);
			paramBegin = itr + 1;
		}
	}
	if (paramBegin != end) ParseParamDecl(paramBegin, end, parent, false);
}

//------------------------------------------------------------------------------
// 仮引数1つ分の解析
// end = ',' | ')'
void HeaderParser::ParseParamDecl(TokenItr begin, TokenItr end, MethodInfoPtr parent, bool isDelegate)
{
	// Identifier, Keyword, Operator, ArithmeticLiteral だけのリストにする
	auto declTokens = tr::MakeEnumerator::from(begin, end)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		.ToList();
	
	auto info = std::make_shared<ParameterInfo>();

	// Delegate の場合
	TokenItr paramEnd;
	if (isDelegate)
	{
		LN_UNREACHABLE();
		//paramEnd = end;

		//// , の直前を引数名とする
		//auto name = paramEnd - 1;

		//// "void(int, int)" の (...) を探す
		//auto paramListBegin = std::find_if(begin, end, [](Token* t) { return t->EqualChar('('); }) + 1;
		//auto paramListEnd = std::find_if(begin, end, [](Token* t) { return t->EqualChar(')'); });

		//// Delegate クラスを作る
		//auto delegateInfo = std::make_shared<TypeInfo>();
		//delegateInfo->isDelegate = true;
		//delegateInfo->name = (*name)->GetString();

		//// Delegate クラスに Invoke メソッドを追加する
		//auto involeMethodInfo = std::make_shared<MethodInfo>();
		//involeMethodInfo->owner = delegateInfo;
		//involeMethodInfo->name = "Invoke";
		//delegateInfo->declaredMethods.Add(involeMethodInfo);

		//// 引数リストを解析する
		//ParseParamListDecl(paramListBegin, paramListEnd, involeMethodInfo);

		//Console::WriteLine(delegateInfo->name);
	}
	// 普通の引数の場合
	else
	{
		paramEnd = std::find_if(declTokens.begin(), declTokens.end(), [](Token* t) { return t->EqualChar('='); });
		if (paramEnd != declTokens.end())
		{
			// デフォルト引数の解析
			String value;
			for (auto itr = paramEnd + 1; itr < declTokens.end(); ++itr) value += (*itr)->GetString();
			info->rawDefaultValue = value;
		}

		// , または =(デフォルト引数) の直前を引数名とする
		auto name = paramEnd - 1;


		String typeName;
		int pointerLevel;
		bool hasConst;
		bool hasVirtual;
		ParseParamType(declTokens.begin(), paramEnd - 1, &typeName, &pointerLevel, &hasConst, &hasVirtual);

		info->name = String((*name)->GetString());
		info->typeRawName = typeName;
		info->isIn = hasConst;
		info->isOut = (!hasConst && pointerLevel > 0);
	}
	parent->parameters.Add(info);

	// copydoc 用シグネチャの抽出
	String sig;
	for (auto itr = declTokens.begin(); itr < paramEnd; ++itr)
	{
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier ||
			(*itr)->GetTokenGroup() == TokenGroup::Operator ||
			(*itr)->GetTokenGroup() == TokenGroup::Keyword)
		{
			sig += (*itr)->GetString();
		}
	}
	if (!parent->paramsRawSignature.IsEmpty()) parent->paramsRawSignature += ",";
	parent->paramsRawSignature += sig;
}

void HeaderParser::ParseParamType(TokenItr begin, TokenItr end, String* outName, int* outPointerLevel, bool* outHasConst, bool* outHasVirtual)
{
	auto type = begin;
	auto typeEnd = end;

	// lookup TypeName and count '*'
	TokenItr typeName;
	int pointerLevel = 0;
	bool hasConst = false;
	bool hasVirtual = false;
	for (auto itr = begin; itr != typeEnd; ++itr)
	{
		if ((*itr)->EqualChar('*')) pointerLevel++;
		if ((*itr)->EqualString("const")) hasConst = true;
		if ((*itr)->EqualString("virtual")) hasVirtual = true;
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier || (*itr)->GetTokenGroup() == TokenGroup::Keyword) typeName = itr;
	}

	*outName = String((*typeName)->GetString());
	*outPointerLevel = pointerLevel;
	*outHasConst = hasConst;
	*outHasVirtual = hasVirtual;
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
			LN_PARSE_RESULT(r3, Many(Parser<TypeName>(Parse_BaseTypeNameLater)));
			auto list = r2.GetValue();
			list.AddRange(r3.GetValue());
			return input.Success(list);
		}
		
		static ParserResult<TypeName> Parse_BaseTypeName(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("public"));
			LN_PARSE_RESULT(r2, Token(TokenGroup::Identifier));
			LN_PARSE_RESULT(r3, Optional(Parser<List<flString>>(Parse_TypeParamList)));
			return input.Success(TypeName{ r2.GetValue()->GetString(), r3.GetValue().GetValue() });
		}

		static ParserResult<TypeName> Parse_BaseTypeNameLater(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(','));
			LN_PARSE_RESULT(r2, TokenString("public"));
			LN_PARSE_RESULT(r3, Token(TokenGroup::Identifier));
			LN_PARSE_RESULT(r4, Optional(Parser<List<flString>>(Parse_TypeParamList)));
			return input.Success(TypeName{ r3.GetValue()->GetString(), r4.GetValue().GetValue() });
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

	m_currentAccessLevel = AccessLevel::Private;
	for (auto& d : decl.decls)
	{
		if (d.type == "AccessLevelLike") ParseAccessLevel(d);
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

	m_currentEnumValue = 0;
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
		m_currentEnumValue = (*value)->GetString().ToInt32();
	}

	// create info
	auto info = std::make_shared<ConstantInfo>();
	info->document = MoveLastDocument();
	info->name = String((*name)->GetString());
	info->value = m_currentEnumValue;
	info->type = parent;
	parent->declaredConstants.Add(info);

	m_currentEnumValue++;
}

//------------------------------------------------------------------------------
// end = ';'
// LN_DELEGATE() using CollisionEventHandler = Delegate<void(PhysicsObject* obj)>;
void HeaderParser::ParseDelegateDecl(const Decl& decl)
{
	// parse "LN_DELEGATE()"
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);

	// '=' を探す。また、その直前の識別子を delegate 名とする。
	TokenItr equal;
	TokenItr name;
	for (auto itr = paramEnd; itr != decl.end; ++itr)
	{
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier)
		{
			name = itr;
		}
		if ((*itr)->EqualChar('='))
		{
			equal = itr;
			break;
		}
	}

	// '(' を探す
	auto lparen = std::find_if(equal, decl.end, [](Token* t) { return t->EqualChar('('); });

	// ')' を探す
	auto rparen = std::find_if(lparen, decl.end, [](Token* t) { return t->EqualChar(')'); });

	// Delegate クラスを作る
	auto delegateInfo = std::make_shared<TypeInfo>();
	delegateInfo->isDelegate = true;
	delegateInfo->document = MoveLastDocument();
	delegateInfo->metadata = MoveLastMetadata();
	delegateInfo->name = (*name)->GetString();
	m_database->delegates.Add(delegateInfo);

	// Delegate クラスに Invoke メソッドを追加する
	auto invokeMethodInfo = std::make_shared<MethodInfo>();
	invokeMethodInfo->owner = delegateInfo;
	invokeMethodInfo->document = std::make_shared<DocumentInfo>();
	invokeMethodInfo->metadata = std::make_shared<MetadataInfo>();
	invokeMethodInfo->returnTypeRawName = "void";
	invokeMethodInfo->name = "Invoke";
	delegateInfo->declaredMethods.Add(invokeMethodInfo);

	// 引数リストを解析する
	ParseParamListDecl(lparen, rparen, invokeMethodInfo);
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
			else if (result[1] == _T("copydoc"))
			{
				String con = line.Mid(result.GetLength());
				if (Regex::Search(con, R"((\w+)(.*))", &result))
				{
					info->copydocMethodName = result[1];
					info->copydocSignature = result[2];
					info->copydocSignature = info->copydocSignature.Remove('(').Remove(')').Remove(' ').Remove('\t');
					target = &info->details;
					line.Clear();
				}
			}
		}

		(*target) += line.Trim();
	}

	m_lastDocument = info;
}

void HeaderParser::ParseAccessLevel(const Decl& decl)
{
	auto t = decl.begin - 1;
	if ((*t)->EqualString("public")) m_currentAccessLevel = AccessLevel::Public;
	if ((*t)->EqualString("protected")) m_currentAccessLevel = AccessLevel::Protected;
	if ((*t)->EqualString("private")) m_currentAccessLevel = AccessLevel::Private;
	if ((*t)->EqualString("internal")) m_currentAccessLevel = AccessLevel::Internal;
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
