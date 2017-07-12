
#pragma once
#include "../Internal.h"

#if defined(LN_OS_WIN32)
	#define INITGUID
#endif

#include <Lumino/Base/Common.h>
#include <Lumino/Base/Logger.h>
#include <Lumino/Base/Exception.h>

#if defined(LN_OS_WIN32)
	#define NOMINMAX
	#include <windows.h>
	#include <mmreg.h>
	#include <mmiscapi.h>
	#include <msacm.h>
	#include <xaudio2.h>
	#include <x3daudio.h>
	#include <d3dx9math.h>
#endif

LN_NAMESPACE_BEGIN
LN_NAMESPACE_END
