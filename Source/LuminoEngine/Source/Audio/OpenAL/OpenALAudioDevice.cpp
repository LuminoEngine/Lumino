
#include "../Internal.h"
#include "OpenALAudioPlayer.h"
#include "OpenALAudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// OpenALAudioDevice
//==============================================================================

//------------------------------------------------------------------------------
OpenALAudioDevice::OpenALAudioDevice()
	: m_metreUnitDistanceInv(1.0f)
{
}

//------------------------------------------------------------------------------
OpenALAudioDevice::OpenALAudioDevice()
{
}

//------------------------------------------------------------------------------
bool OpenALAudioDevice::Initialize()
{
	return false;
}

//------------------------------------------------------------------------------
AudioPlayer* OpenALAudioDevice::CreateAudioPlayer(AudioStream* audioStream, bool enable3d, SoundPlayingMode mode)
{
	RefPtr<AudioPlayer> audioPlayer;

    // 種類に応じてプレイヤーを作成する
	switch ( mode )
	{
		// オンメモリ再生
		case SoundPlayingMode::OnMemory:
        {
			OpenALOnMemoryAudioPlayer* player = LN_NEW OpenALOnMemoryAudioPlayer(this);
			audioPlayer.Attach(player, false);
			player->Initialize(audioStream, enable3d);
			break;
        }
		// ストリーミング再生
		case SoundPlayingMode::Streaming:
        {
			OpenALStreamingAudioPlayer* player = LN_NEW OpenALStreamingAudioPlayer(this);
			audioPlayer.Attach(player, false);
			player->Initialize(audioStream, enable3d);
			break;
        }
		default:
			LN_THROW(0, ArgumentException);
			break;
	}

	audioPlayer.SafeAddRef();
	return audioPlayer;
}

//------------------------------------------------------------------------------
void OpenALAudioDevice::Update()
{
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
