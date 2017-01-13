
#pragma once
#include <memory>
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/ParserCombinators.h>
#include "SymbolDatabase.h"

namespace fl { class TokenList; }

/** */
class HeaderParser
{
public:
	using TokenItr = fl::combinators::Iterator;

	HeaderParser(SymbolDatabase* database);
	void ParseFiles(const List<PathName>& pathes);
	
private:
	struct Decl
	{
		String		type;
		TokenItr	begin;
		TokenItr	end;
		List<Decl>	decls;
	};

	void ParseFile(const PathName& path);
	TokenItr ParseMetadataDecl(TokenItr begin, TokenItr end);
	void ParseStructDecl(const Decl& decl);
	void ParseFieldDecl(const Decl& decl, TypeInfoPtr parent);
	void ParseMethodDecl(const Decl& decl, TypeInfoPtr parent);
	void ParseParamsDecl(TokenItr begin, TokenItr end, MethodInfoPtr parent);
	void ParseClassDecl(const fl::TokenList* tokens, int begin, int end);

	SymbolDatabase*	m_database;
};

