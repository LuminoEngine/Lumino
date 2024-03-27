#include "Internal.hpp"
#include <LuminoFramework/Audio/GameAudio.hpp>
#include "GameAudioImpl.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// GameAudio

static detail::GameAudioImpl* coreInstance()
{
    return detail::EngineDomain::audioManager()->gameAudio();
}

void GameAudio::playBGM(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    coreInstance()->playBGM(filePath, volume, pitch, fadeTime);
}

void GameAudio::stopBGM(double fadeTime)
{
    coreInstance()->stopBGM(fadeTime);
}

void GameAudio::playBGS(const StringView& filePath, float volume, float pitch, double fadeTime)
{
    coreInstance()->playBGS(filePath, volume, pitch, fadeTime);
}

void GameAudio::stopBGS(double fadeTime)
{
    coreInstance()->stopBGS(fadeTime);
}

void GameAudio::playME(const StringView& filePath, float volume, float pitch)
{
    coreInstance()->playME(filePath, volume, pitch);
}

void GameAudio::stopME()
{
    coreInstance()->stopME();
}

void GameAudio::playSE(const StringView& filePath, float volume, float pitch)
{
    coreInstance()->playSE(filePath, volume, pitch);
}

void GameAudio::playSE3D(const StringView& filePath, const Vector3& position, float distance, float volume, float pitch)
{
    coreInstance()->playSE3D(filePath, position, distance, volume, pitch);
}

void GameAudio::stopSE()
{
    coreInstance()->stopSE();
}

void GameAudio::setMEFadeState(double begin, double end)
{
    coreInstance()->setMEFadeState(begin, end);
}

void GameAudio::setBGMVolume(float volume, double fadeTime)
{
    coreInstance()->setBGMVolume(volume, fadeTime);
}

void GameAudio::setBGSVolume(float volume, double fadeTime)
{
    coreInstance()->setBGSVolume(volume, fadeTime);
}

void GameAudio::setEnableBGMRestart(bool enabled)
{
    coreInstance()->setEnableBGMRestart(enabled);
}

void GameAudio::setEnableBGSRestart(bool enabled)
{
    coreInstance()->setEnableBGSRestart(enabled);
}

} // namespace ln

