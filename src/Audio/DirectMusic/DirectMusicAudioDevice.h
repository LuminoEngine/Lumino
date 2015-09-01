
#pragma once

#include "../AudioDevice.h"

namespace Lumino
{
namespace Audio
{

///
class DirectMusicAudioDevice
    : public AudioDevice
{
public:

    struct ConfigData
    {
        DirectMusicInitMode		DMInitMode;		///< DirectMusic を初期化する時の方法
        HWND					hWnd;			///< DirectMusic の初期化に使うウィンドウハンドル
    };

public:
	DirectMusicAudioDevice();
	virtual ~DirectMusicAudioDevice();

public:
    void Initialize( const ConfigData& configData );
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayType type);
	virtual void Update();
	virtual void SetMetreUnitDistance(float d);
};

} // namespace Audio
} // namespace Lumino
