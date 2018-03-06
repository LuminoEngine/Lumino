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

	static StringType getExecutablePath()
	{
		CharType result[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, result, MAX_PATH);
		return result;
	}

	static void getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath)
	{
		LN_CHECK(outPath);

		WCHAR path[MAX_PATH] = {0};
		switch (specialFolder)
		{
		case SpecialFolder::ApplicationData:
			::SHGetSpecialFolderPathW(NULL, path, CSIDL_APPDATA, FALSE);
			break;
		case SpecialFolder::Temporary:
			::GetTempPathW(MAX_PATH, path);
			break;
		default:
			LN_UNREACHABLE();
			break;
		}

		*outPath = path;
	}
};

LN_NAMESPACE_END

