#pragma once
#include <userenv.h>
#include <LuminoCore/Base/Platform.hpp>

namespace ln {

class PlatformEnvironment {
public:
    using CharType = wchar_t;
    using StringType = std::wstring;

    static void getCurrentDirectory(StringType* outPath) {
        LN_CHECK(outPath);
        DWORD size = ::GetCurrentDirectoryW(0, NULL); // size is contains '\0'
        outPath->resize(size - 1);
        ::GetCurrentDirectoryW(size, &((*outPath)[0]));
    }

    static void setCurrentDirectory(const Path& path) {
        BOOL r = ::SetCurrentDirectoryW(path.str().toStdWString().c_str());
        if (LN_ENSURE(r != FALSE, _TT("{}"), detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
    }

    static StringType getExecutablePath() {
        CharType result[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, result, MAX_PATH);
        return result;
    }

    static void setEnvironmentVariable(const StringView& variableName, const StringView& value) {
        BOOL r = ::SetEnvironmentVariableW(variableName.toStdWString().c_str(), value.toStdWString().c_str());
        if (LN_ENSURE(r != FALSE, _TT("{}"), detail::Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
    }

    static void getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath) {
        LN_CHECK(outPath);

        WCHAR path[MAX_PATH] = { 0 };
        switch (specialFolder) {
            case SpecialFolder::ApplicationData:
                ::SHGetSpecialFolderPathW(NULL, path, CSIDL_APPDATA, FALSE);
                break;
            case SpecialFolder::Temporary:
                ::GetTempPathW(MAX_PATH, path);
                break;
            case SpecialFolder::Home: {
                BOOL bResult;
                HANDLE hToken;
                bResult = ::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
                if (bResult) {
                    DWORD dwBufferSize;
                    bResult = ::GetUserProfileDirectoryW(hToken, NULL, &dwBufferSize);
                    if (!bResult && dwBufferSize != 0) {
                        wchar_t* directory = new wchar_t[dwBufferSize];
                        bResult = GetUserProfileDirectoryW(hToken, directory, &dwBufferSize);
                        if (bResult) {
                            *outPath = directory;
                        }
                        delete[] directory;
                    }
                    ::CloseHandle(hToken);
                }
                break;
            }
            default:
                LN_UNREACHABLE();
                break;
        }

        *outPath = path;
    }

    static uint64_t getTickCount() {
        // timeGetTime() は timeBeginPeriod() によって精度が変わるため、
        // GetTickCount() の方が無難かもしれない
        return ::GetTickCount64();
    }
};

} // namespace ln
