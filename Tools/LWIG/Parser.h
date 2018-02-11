
#pragma once
#include <unordered_map>

class SymbolDatabase;
class DocumentInfo;
class MetadataInfo;

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

	int parse(const Path& filePath, ::SymbolDatabase* db);
	AttrMacro* findUnlinkedAttrMacro(unsigned offset);

	::SymbolDatabase* getDB() const { return m_db; }



	static Ref<DocumentInfo> parseDocument(const std::string& comment);
	static Ref<MetadataInfo> parseMetadata(std::string name, const std::string& args);

private:
	::SymbolDatabase*	m_db;
	List<Path>	m_includePathes;
};


