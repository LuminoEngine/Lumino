
#pragma once
#include "LuminoEngine.h"

#if defined(_WIN32)
	#pragma comment(lib, "imagehlp.lib")
	#pragma comment(lib, "msacm32.lib")
	#pragma comment(lib, "Opengl32.lib")
#endif

#if defined(_DEBUG)
	#pragma comment(lib, "Debug/bullet_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/Effekseerd.lib")
	#pragma comment(lib, "Debug/EffekseerRendererDX9d.lib")
	#pragma comment(lib, "Debug/EffekseerRendererGLd.lib")
	#pragma comment(lib, "Debug/EffekseerSoundXAudio2d.lib")
	#pragma comment(lib, "Debug/freetype_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/libogg_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/libpng_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/libvorbis_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/Lumino_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/LuminoCore_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/LuminoFoundation_msvc140x86u_static_MTd.lib")
	#pragma comment(lib, "Debug/LuminoMath_msvc140x86_static_MTd.lib")
	#pragma comment(lib, "Debug/zlib_msvc140x86u_static_MTd.lib")
#else
	#pragma comment(lib, "Release/bullet_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/Effekseer.lib")
	#pragma comment(lib, "Release/EffekseerRendererDX9.lib")
	#pragma comment(lib, "Release/EffekseerRendererGL.lib")
	#pragma comment(lib, "Release/EffekseerSoundXAudio2.lib")
	#pragma comment(lib, "Release/freetype_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/libogg_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/libpng_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/libvorbis_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/Lumino_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/LuminoCore_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/LuminoFoundation_msvc140x86u_static_MT.lib")
	#pragma comment(lib, "Release/LuminoMath_msvc140x86_static_MT.lib")
	#pragma comment(lib, "Release/zlib_msvc140x86u_static_MT.lib")
#endif