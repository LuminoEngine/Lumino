
#pragma once
#include "LuminoEngine.h"

#if defined(_WIN32) && !defined(LUMINO_LINK_LIBRARY_DISABLED)
	#pragma comment(lib, "imagehlp.lib")
	#pragma comment(lib, "msacm32.lib")
	#pragma comment(lib, "Opengl32.lib")

	#if defined(_WIN64)
		#error "x64 architecture is not supported."
	#else
		#if _MSC_VER >= 1900	// Visual C++ 14.0 (Visual Studio 2015)
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
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "bullet_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Effekseerd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererDX9d.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererGLd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerSoundXAudio2d.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "freetype_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libogg_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libpng_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libvorbis_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Lumino_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoCore_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoFoundation_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoMath_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "zlib_" LN_MSVC_ARCH_MSVC "x86_static_MTd.lib")
	#else
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "bullet_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Effekseer.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererDX9.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerRendererGL.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "EffekseerSoundXAudio2.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "freetype_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libogg_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libpng_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "libvorbis_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "Lumino_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoCore_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoFoundation_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "LuminoMath_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
		#pragma comment(lib, LN_MSVC_ARCH_BASE_PATH "zlib_" LN_MSVC_ARCH_MSVC "x86_static_MT.lib")
	#endif

#endif

