#pragma once

namespace ln {

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

	static StringType getExecutablePath()
	{
		LN_NOTIMPLEMENTED();
		return StringType();
	}
};

} // namespace ln

