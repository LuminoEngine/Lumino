
#include "Internal.hpp"
#include <LuminoEngine/Audio/Audio.hpp>
#include "GameAudioImpl.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// Audio

static detail::GameAudioImpl2* coreInstance()
{
    return detail::EngineDomain::audioManager()->gameAudio2();
}

void Audio::playBGM(const StringRef& filePath, float volume, float pitch, double fadeTime)
{
    coreInstance()->playBGM(filePath, volume, pitch, fadeTime);
}

void Audio::stopBGM(double fadeTime)
{
    coreInstance()->stopBGM(fadeTime);
}

void Audio::playBGS(const StringRef& filePath, float volume, float pitch, double fadeTime)
{
    coreInstance()->playBGS(filePath, volume, pitch, fadeTime);
}

void Audio::stopBGS(double fadeTime)
{
    coreInstance()->stopBGS(fadeTime);
}

void Audio::playME(const StringRef& filePath, float volume, float pitch)
{
    coreInstance()->playME(filePath, volume, pitch);
}

void Audio::stopME()
{
    coreInstance()->stopME();
}

void Audio::playSE(const StringRef& filePath, float volume, float pitch)
{
    coreInstance()->playSE(filePath, volume, pitch);
}

void Audio::playSE3D(const StringRef& filePath, const Vector3& position, float distance, float volume, float pitch)
{
    coreInstance()->playSE3D(filePath, position, distance, volume, pitch);
}

void Audio::stopSE()
{
    coreInstance()->stopSE();
}

//void Audio::setMEFadeState(double begin, double end)
//{
//    coreInstance()->setMEFadeState(begin, end);
//}
//
//void Audio::setBGMVolume(float volume, double fadeTime)
//{
//    coreInstance()->setBGMVolume(volume, fadeTime);
//}
//
//void Audio::setBGSVolume(float volume, double fadeTime)
//{
//    coreInstance()->setBGSVolume(volume, fadeTime);
//}
//
//void Audio::setEnableBGMRestart(bool enabled)
//{
//    coreInstance()->setEnableBGMRestart(enabled);
//}
//
//void Audio::setEnableBGSRestart(bool enabled)
//{
//    coreInstance()->setEnableBGSRestart(enabled);
//}
//
} // namespace ln

