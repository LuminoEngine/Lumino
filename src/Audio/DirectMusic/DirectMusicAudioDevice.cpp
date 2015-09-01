
#include "../Internal.h"
#include "../MidiDecoder.h"
#include "DirectMusic.h"
#include "DirectMusicAudioPlayer.h"
#include "DirectMusicAudioDevice.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// DirectMusicAudioDevice
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DirectMusicAudioDevice::DirectMusicAudioDevice()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DirectMusicAudioDevice::~DirectMusicAudioDevice()
{
	if (DirectMusicManager::GetInstance())
	{
		DirectMusicManager::GetInstance()->Finalize();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicAudioDevice::Initialize( const ConfigData& configData )
{
    // DirectMusic を初期化する場合
	if (configData.DMInitMode != DirectMusicInitMode_NotUse)
    {
        DirectMusicManager::ConfigData dm_data;
        dm_data.DMInitMode		= configData.DMInitMode;
		dm_data.WindowHandle	= configData.hWnd;
        DirectMusicManager::Initialize(dm_data);
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer* DirectMusicAudioDevice::CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayType type)
{
	RefPtr<DirectMusicAudioPlayer> audioPlayer;

	if (type == SoundPlayType_Midi)
    {
		LN_THROW(DirectMusicManager::GetInstance(), InvalidOperationException);
		audioPlayer.Attach(LN_NEW DirectMusicAudioPlayer(this), false);
		audioPlayer->Initialize(source, enable3d);
    }

	audioPlayer.SafeAddRef();
	return audioPlayer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicAudioDevice::Update()
{
    if ( DirectMusicManager::GetInstance() )
    {
        DirectMusicManager::GetInstance()->Polling();
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicAudioDevice::SetMetreUnitDistance(float d)
{
}

} // namespace Audio
} // namespace Lumino
