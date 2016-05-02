
#include "Internal.h"
#include "NullAudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//=============================================================================
// NullAudioPlayer
//=============================================================================

//-----------------------------------------------------------------------------
NullAudioPlayer::NullAudioPlayer(AudioDevice* device)
	: AudioPlayer(device)
{
}

//-----------------------------------------------------------------------------
NullAudioPlayer::~NullAudioPlayer()
{
}


//=============================================================================
// NullAudioDevice
//=============================================================================

//-----------------------------------------------------------------------------
NullAudioDevice::NullAudioDevice()
{
}

//-----------------------------------------------------------------------------
NullAudioDevice::~NullAudioDevice()
{
}

//-----------------------------------------------------------------------------
AudioPlayer* NullAudioDevice::CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode)
{
	RefPtr<NullAudioPlayer> player(LN_NEW NullAudioPlayer(this), false);
	player->Initialize(source, enable3d);
	return player.DetachMove();
}

//-----------------------------------------------------------------------------
void NullAudioDevice::Update()
{
}

//-----------------------------------------------------------------------------
void NullAudioDevice::SetMetreUnitDistance(float d)
{
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END

