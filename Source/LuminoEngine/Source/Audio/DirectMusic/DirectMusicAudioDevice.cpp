
#include "../Internal.h"
#include "../MidiDecoder.h"
#include "DirectMusic.h"
#include "DirectMusicAudioPlayer.h"
#include "DirectMusicAudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// DirectMusicAudioDevice
//==============================================================================

//------------------------------------------------------------------------------
DirectMusicAudioDevice::DirectMusicAudioDevice()
{
}

//------------------------------------------------------------------------------
DirectMusicAudioDevice::~DirectMusicAudioDevice()
{
	if (DirectMusicManager::GetInstance())
	{
		DirectMusicManager::GetInstance()->Finalize();
	}
}

//------------------------------------------------------------------------------
void DirectMusicAudioDevice::initialize( const ConfigData& configData )
{
    // DirectMusic を初期化する場合
	if (configData.DMInitMode != DirectMusicMode::NotUse)
    {
        DirectMusicManager::ConfigData dm_data;
        dm_data.DMInitMode		= configData.DMInitMode;
		dm_data.WindowHandle	= configData.hWnd;
		dm_data.ReverbLevel		= configData.ReverbLevel;
        DirectMusicManager::initialize(dm_data);
    }
}

//------------------------------------------------------------------------------
AudioPlayer* DirectMusicAudioDevice::CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode)
{
	RefPtr<DirectMusicAudioPlayer> audioPlayer;

	if (mode == SoundPlayingMode::Midi)
    {
		LN_THROW(DirectMusicManager::GetInstance(), InvalidOperationException);
		audioPlayer.attach(LN_NEW DirectMusicAudioPlayer(this), false);
		audioPlayer->initialize(source, enable3d);
    }

	audioPlayer.SafeAddRef();
	return audioPlayer;
}

//------------------------------------------------------------------------------
void DirectMusicAudioDevice::Update()
{
    if ( DirectMusicManager::GetInstance() )
    {
        DirectMusicManager::GetInstance()->Polling();
    }
}

//------------------------------------------------------------------------------
void DirectMusicAudioDevice::SetMetreUnitDistance(float d)
{
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
