
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
		flString	type;
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
	void ParseParamType(TokenItr begin, TokenItr end, String* outName, int* outPointerLevel, bool* outHasConst, bool* outHasVirtual);
	void ParseClassDecl(const Decl& decl);
	void ParseEnumDecl(const Decl& decl);
	void ParseEnumMemberDecl(TokenItr begin, TokenItr end, TypeInfoPtr parent);
	void ParseDocument(const Decl& decl);
	void ParseAccessLevel(const Decl& decl);

	DocumentInfoPtr MoveLastDocument();
	MetadataInfoPtr MoveLastMetadata();

	SymbolDatabase*	m_database;
	DocumentInfoPtr	m_lastDocument;
	MetadataInfoPtr	m_lastMetadata;
	AccessLevel		m_currentAccessLevel;
};

