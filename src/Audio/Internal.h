
#pragma once

#define INITGUID

#include <Lumino/Base/Common.h>
#include <Lumino/Base/Logger.h>
#include <Lumino/Base/Exception.h>

#ifdef LN_WIN32
	#include <mmreg.h>
	#include <msacm.h>
	#include <xaudio2.h>
	#include <x3daudio.h>
	#include <d3dx9math.h>
	
#endif

#define LN_CLAMP(value, minValue, maxValue)		(value > maxValue) ? maxValue : (value < minValue) ? minValue : value;

namespace Lumino
{
namespace Audio
{
class AudioManager;

class Internal
{
public:
	static AudioManager* Manager;
};

}
}