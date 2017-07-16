
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
	if (DirectMusicManager::getInstance())
	{
		DirectMusicManager::getInstance()->dispose();
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
AudioPlayer* DirectMusicAudioDevice::createAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode)
{
	Ref<DirectMusicAudioPlayer> audioPlayer;

	if (mode == SoundPlayingMode::Midi)
    {
		LN_THROW(DirectMusicManager::getInstance(), InvalidOperationException);
		audioPlayer.attach(LN_NEW DirectMusicAudioPlayer(this), false);
		audioPlayer->initialize(source, enable3d);
    }

	audioPlayer.safeAddRef();
	return audioPlayer;
}

//------------------------------------------------------------------------------
void DirectMusicAudioDevice::update()
{
    if ( DirectMusicManager::getInstance() )
    {
        DirectMusicManager::getInstance()->polling();
    }
}

//------------------------------------------------------------------------------
void DirectMusicAudioDevice::setMetreUnitDistance(float d)
{
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
