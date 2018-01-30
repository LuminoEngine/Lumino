#pragma once

LN_NAMESPACE_BEGIN

// Unix
class PlatformEnvironment
{
public:
	using CharType = char;
	using StringType = std::string;
	
	static void getCurrentDirectory(StringType* outPath)
	{
		char* p = getcwd(NULL, 0);
		size_t len = strlen(p);
		outPath->resize(len);
		strncpy(&((*outPath)[0]), p, len);
		free(p);
	}
	
};

LN_NAMESPACE_END

