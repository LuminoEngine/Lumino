
#include "../Internal.h"
#include <Lumino/Audio/Sound.h>
#include <Lumino/Audio/GameAudio.h>
#include "AudioManager.h"
#include "GameAudioImpl.h"
#include "AudioHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

enum GameAudioFlags
{
	GameAudioFlags_SE = 0x01,
};

//=============================================================================
// GameAudio
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGM(const TCHAR* filePath, float volume, float pitch, double fadeTime)
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayBGM(filePath, volume, pitch, fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayBGMFromSound(sound, volume, pitch, fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopBGM(double fadeTime)
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->StopBGM(fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGS( const TCHAR* filePath, float volume, float pitch, double fadeTime )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayBGS(filePath, volume, pitch, fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayBGSFromSound( Sound* sound, float volume, float pitch, double fadeTime )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayBGSFromSound(sound, volume, pitch, fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopBGS( double fadeTime )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->StopBGS(fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayME( const TCHAR* filePath, float volume, float pitch )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayME(filePath, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlayMEFromSound( Sound* sound, float volume, float pitch )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlayMEFromSound(sound, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopME()
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->StopBGS();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySE( const TCHAR* filePath, float volume, float pitch )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlaySE(filePath, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySE3D( const TCHAR* filePath, const Vector3& position, float distance, float volume, float pitch )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlaySE3D(filePath, position, distance, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::PlaySEFromSound( Sound* srcSound, float volume, float pitch )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->PlaySEFromSound(srcSound, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::StopSE()
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->StopSE();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetMEFadeState(double begin, double end)
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->SetMEFadeState(begin, end);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetBGMVolume( float volume, double fadeTime )
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->SetBGMVolume(volume, fadeTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudio::SetBGSVolume(float volume, double fadeTime)
{
	AudioManagerImpl::GetInstance()->GetGameAudio()->SetBGSVolume(volume, fadeTime);
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
