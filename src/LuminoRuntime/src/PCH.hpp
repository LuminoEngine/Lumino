#pragma once
#define LUMINO_BUILD_DLL
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>

#ifdef LN_OS_WIN32
#define NOMINMAX
#include <Windows.h>
#endif
