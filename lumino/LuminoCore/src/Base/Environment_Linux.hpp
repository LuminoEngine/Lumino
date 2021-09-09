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
#if defined(LN_EMSCRIPTEN) || defined(LN_OS_ANDROID)
		return "/";
#else
		LN_NOTIMPLEMENTED();
		return StringType();
#endif
	}

	static void setEnvironmentVariable(const StringRef& variableName, const StringRef& value)
	{
		std::string str = ln::String::format(_TT("{0}={1}"), variableName, value).toStdString();
		std::vector<char> buf(str.c_str(), str.c_str() + str.length());
		putenv(buf.data());
	}

	static void getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath)
	{
		LN_NOTIMPLEMENTED();
	}

    static uint64_t getTickCount()
    {
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
            return 0;
        }
        return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    }
};

} // namespace ln

