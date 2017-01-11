
#pragma once

namespace fl { class TokenList; }

/** */
class HeaderParser
{
public:
	void ParseFiles(const List<PathName>& pathes);
	
private:
	void ParseFile(const PathName& path);
	void ParseClassDecl(const fl::TokenList* tokens, int begin, int end);
};

