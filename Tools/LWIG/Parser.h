
#pragma once
#include <unordered_map>

class SymbolDatabase;
class DocumentInfo;

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

	int parse(const Path& filePath, ::SymbolDatabase* db);
	AttrMacro* findUnlinkedAttrMacro(unsigned offset);

	::SymbolDatabase* getDB() const { return m_db; }



	static Ref<DocumentInfo> parseDocument(const std::string& comment);
private:

	::SymbolDatabase*	m_db;
};


