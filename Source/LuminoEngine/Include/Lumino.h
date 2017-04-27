
#pragma once
#include "LuminoEngine.h"

#if defined(_WIN32) && !defined(LUMINO_LINK_LIBRARY_DISABLED)
	#pragma comment(lib, "imagehlp.lib")
	#pragma comment(lib, "msacm32.lib")
	#pragma comment(lib, "Opengl32.lib")

	#if defined(LN_DEBUG)
		#define LN_MSVC_LIBRARY_CONFIGURATION_NAME	"Debug"
	#else
		#define LN_MSVC_LIBRARY_CONFIGURATION_NAME	"Release"
	#endif

	#if defined(_WIN64)
		#error "x64 architecture is not supported."
	#else
		#if _MSC_VER >= 1910	// Visual C++ 15.0 (Visual Studio 2017)
			#define LN_MSVC_ARCH_BASE_PATH	"MSVC150_x86_MT/" LN_MSVC_LIBRARY_CONFIGURATION_NAME "/"
			#define LN_MSVC_ARCH_MSVC		"msvc150"
		#elif _MSC_VER >= 1900	// Visual C++ 14.0 (Visual Studio 2015)
			#define LN_MSVC_ARCH_BASE_PATH	"MSVC140/x86/"
			#define LN_MSVC_ARCH_MSVC		"msvc140"
		#elif _MSC_VER >= 1800	// Visual C++ 12.0 (Visual Studio 2013)
			#define LN_MSVC_ARCH_BASE_PATH	"MSVC120/x86/"
			#define LN_MSVC_ARCH_MSVC		"msvc120"
		#else
			#error "current MSVC is not supported."
		#endif
	#endif

	#ifdef _DEBUG
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "bullet.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Effekseer.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererDX9.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererGL.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerSoundXAudio2.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "freetype.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libogg.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libpng.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libvorbis.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoEngine.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoCore.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoLaunch.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Fluorite.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "zlib.lib")
	#else
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "bullet.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Effekseer.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererDX9.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererGL.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerSoundXAudio2.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "freetype.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libogg.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libpng.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libvorbis.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoEngine.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoCore.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoLaunch.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Fluorite.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "zlib.lib")
	#endif

#endif

