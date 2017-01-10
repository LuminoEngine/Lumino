
#pragma once

/** */
class HeaderParser
{
public:
	void ParseFiles(const List<PathName>& pathes);
	
private:
	void ParseFile(const PathName& path);
};

