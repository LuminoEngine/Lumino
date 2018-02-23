
#pragma once
#include <unordered_map>

class SymbolDatabase;
class DocumentSymbol;
class MetadataSymbol;
class DiagManager;

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


	void addIncludePath(const Path& path) { m_includePathes.add(path); }

	int parse(const Path& filePath, ::SymbolDatabase* db, DiagManager* diag);
	AttrMacro* findUnlinkedAttrMacro(unsigned offset);

	::SymbolDatabase* getDB() const { return m_db; }
	DiagManager* diag() const { return m_diag; }



	static Ref<DocumentSymbol> parseDocument(const std::string& comment);
	static Ref<MetadataSymbol> parseMetadata(std::string name, const std::string& args);

private:
	::SymbolDatabase*	m_db;
	List<Path>	m_includePathes;
	DiagManager* m_diag;
};


