
#pragma once

#if defined(LN_BUILD_DLL) && defined(LN_MSVC)
	#define LN_API	__declspec(dllexport)
#else
	#define LN_API
#endif

typedef char LNChar;

%Enums%

