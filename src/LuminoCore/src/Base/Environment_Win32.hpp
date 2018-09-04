#pragma once
#include <Lumino/Base/Platform.hpp>

namespace ln {

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

	static void setEnvironmentVariable(const StringRef& variableName, const StringRef& value)
	{
		BOOL r = ::SetEnvironmentVariableW(variableName.toStdWString().c_str(), value.toStdWString().c_str());
		if (LN_ENSURE(r != FALSE, detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
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

} // namespace ln

