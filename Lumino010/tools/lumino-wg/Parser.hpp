
#pragma once
#include <unordered_map>
#include "SymbolDatabase.hpp"

// ひとつのヘッダファイルを解析して、SymbolDatabase に情報を追加する。
class HeaderParser
{
public:
	struct AttrMacro
	{
		unsigned offset = 0;
		std::string name;
		std::string args;
		bool linked = false;
	};

	std::vector<AttrMacro>	lnAttrMacros;		// <ファイル先頭からのオフセット, 属性情報>


	void addIncludePath(const ln::Path& path) { m_includePathes.add(path); }
	void addForceIncludeFile(const ln::Path& path) { m_forceIncludeFiles.add(path); }


	int parse(const ln::Path& filePath, ::SymbolDatabase* db, ln::DiagnosticsManager* diag);
	AttrMacro* findUnlinkedAttrMacro(unsigned offset);

	::SymbolDatabase* getDB() const { return m_db; }
	ln::DiagnosticsManager* diag() const { return m_diag; }



	static ln::Ref<DocumentSymbol> parseDocument(const std::string& comment);
	static ln::Ref<MetadataSymbol> parseMetadata(std::string name, const std::string& args);

private:
	::SymbolDatabase*	m_db;
	ln::List<ln::Path>	m_includePathes;
	ln::List<ln::Path>	m_forceIncludeFiles;
	ln::DiagnosticsManager* m_diag;
};


