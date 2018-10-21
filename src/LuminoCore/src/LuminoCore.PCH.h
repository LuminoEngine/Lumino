#pragma once
#define LUMINO_BUILD_DLL
#define LN_INTERNAL_ACCESS public
#define LN_PROTECTED_INTERNAL_ACCESS public
#include <LuminoCore/Base/Common.hpp>

#if defined(LN_OS_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif
