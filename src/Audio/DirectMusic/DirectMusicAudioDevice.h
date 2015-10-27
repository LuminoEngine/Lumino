
#pragma once

#include "../AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

///
class DirectMusicAudioDevice
    : public AudioDevice
{
public:

    struct ConfigData
    {
        DirectMusicMode		DMInitMode;		///< DirectMusic を初期化する時の方法
        HWND				hWnd;			///< DirectMusic の初期化に使うウィンドウハンドル
		float				ReverbLevel;
    };

public:
	DirectMusicAudioDevice();
	virtual ~DirectMusicAudioDevice();

public:
    void Initialize( const ConfigData& configData );
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode);
	virtual void Update();
	virtual void SetMetreUnitDistance(float d);
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
