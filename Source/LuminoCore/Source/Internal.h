#pragma once

#if defined(LN_GNUC) || defined(__CYGWIN__)
#include <unistd.h>
//#include <Base/CRT.h>
#endif

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#endif

/// char mbcsPath[LN_MAX_PATH + 1] をローカル変数として作成し、wchar_t 文字列を変換して格納する (Unix 系のファイルアクセス用)
#define LN_LOCAL_MBCS_FILEPATH(mbcsPath, srcWPath) \
	char mbcsPath[LN_MAX_PATH + 1]; \
	if (wcstombs(mbcsPath, srcWPath, LN_MAX_PATH) < 0) { \
		LN_THROW(0, ArgumentException); \
	}
