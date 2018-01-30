#pragma once

LN_NAMESPACE_BEGIN

class PlatformEnvironment
{
public:
	using CharType = wchar_t;
	using StringType = std::wstring;

	static void getCurrentDirectory(StringType* outPath)
	{
		LN_CHECK(outPath);
		DWORD size = ::GetCurrentDirectoryW(0, NULL);	// size is contains '\0'
		outPath->resize(size - 1);
		::GetCurrentDirectoryW(size, &((*outPath)[0]));
	}
};

LN_NAMESPACE_END

