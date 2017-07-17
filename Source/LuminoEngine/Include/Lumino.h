
#pragma once
#include "LuminoEngine.h"

#if defined(_WIN32) && !defined(LUMINO_LINK_LIBRARY_DISABLED)
	#pragma comment(lib, "imagehlp.lib")
	#pragma comment(lib, "msacm32.lib")
	#pragma comment(lib, "Opengl32.lib")
	
	#if defined(_DEBUG)
		#define LN_MSVC_LIBRARY_POSTFIX "d.lib"
	#else
		#define LN_MSVC_LIBRARY_POSTFIX ".lib"
	#endif

	#pragma comment(lib, "bullet" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "Effekseer" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "EffekseerRendererDX9" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "EffekseerRendererGL" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "EffekseerSoundXAudio2" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "freetype" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "giflib" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "libogg" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "libpng" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "libvorbis" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "LuminoCore" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "LuminoEngine" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "LuminoExperimental" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "LuminoLaunch" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "Fluorite" LN_MSVC_LIBRARY_POSTFIX)
	#pragma comment(lib, "zlib" LN_MSVC_LIBRARY_POSTFIX)
#endif

