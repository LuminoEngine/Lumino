
#pragma once
#include "../AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

// 実際に再生を行わないダミーインターフェイス
class NullAudioDevice
    : public AudioDevice
{
public:
	NullAudioDevice();
	virtual ~NullAudioDevice();
	
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END

