
#include "../Internal.h"
#include <Lumino/Audio/Sound.h>
#include <Lumino/Audio/GameAudio.h>
#include "AudioManager.h"
#include "GameAudioImpl.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

enum GameAudioFlags
{
	GameAudioFlags_SE = 0x01,
};

//==============================================================================
// GameAudio
//==============================================================================

//------------------------------------------------------------------------------
void GameAudio::playBGM(const Char* filePath, float volume, float pitch, double fadeTime)
{
	detail::AudioManager::getInstance()->getGameAudio()->playBGM(filePath, volume, pitch, fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::playBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	detail::AudioManager::getInstance()->getGameAudio()->playBGMFromSound(sound, volume, pitch, fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::stopBGM(double fadeTime)
{
	detail::AudioManager::getInstance()->getGameAudio()->stopBGM(fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::playBGS( const Char* filePath, float volume, float pitch, double fadeTime )
{
	detail::AudioManager::getInstance()->getGameAudio()->playBGS(filePath, volume, pitch, fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::playBGSFromSound( Sound* sound, float volume, float pitch, double fadeTime )
{
	detail::AudioManager::getInstance()->getGameAudio()->playBGSFromSound(sound, volume, pitch, fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::stopBGS( double fadeTime )
{
	detail::AudioManager::getInstance()->getGameAudio()->stopBGS(fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::playME( const Char* filePath, float volume, float pitch )
{
	detail::AudioManager::getInstance()->getGameAudio()->playME(filePath, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudio::playMEFromSound( Sound* sound, float volume, float pitch )
{
	detail::AudioManager::getInstance()->getGameAudio()->playMEFromSound(sound, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudio::stopME()
{
	detail::AudioManager::getInstance()->getGameAudio()->stopBGS();
}

//------------------------------------------------------------------------------
void GameAudio::playSE( const Char* filePath, float volume, float pitch )
{
	detail::AudioManager::getInstance()->getGameAudio()->playSE(filePath, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudio::playSE3D( const Char* filePath, const Vector3& position, float distance, float volume, float pitch )
{
	detail::AudioManager::getInstance()->getGameAudio()->playSE3D(filePath, position, distance, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudio::playSEFromSound( Sound* srcSound, float volume, float pitch )
{
	detail::AudioManager::getInstance()->getGameAudio()->playSEFromSound(srcSound, volume, pitch);
}

//------------------------------------------------------------------------------
void GameAudio::stopSE()
{
	detail::AudioManager::getInstance()->getGameAudio()->stopSE();
}

//------------------------------------------------------------------------------
void GameAudio::setMEFadeState(double begin, double end)
{
	detail::AudioManager::getInstance()->getGameAudio()->setMEFadeState(begin, end);
}

//------------------------------------------------------------------------------
void GameAudio::setBGMVolume( float volume, double fadeTime )
{
	detail::AudioManager::getInstance()->getGameAudio()->setBGMVolume(volume, fadeTime);
}

//------------------------------------------------------------------------------
void GameAudio::setBGSVolume(float volume, double fadeTime)
{
	detail::AudioManager::getInstance()->getGameAudio()->setBGSVolume(volume, fadeTime);
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
