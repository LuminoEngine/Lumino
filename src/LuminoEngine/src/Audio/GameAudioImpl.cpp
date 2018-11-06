
#include "Internal.hpp"
#include "GameAudioImpl.hpp"
#include "AudioManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GameAudioImpl

GameAudioImpl::GameAudioImpl(AudioManager* mamager)
{
}

GameAudioImpl::~GameAudioImpl()
{
}

void GameAudioImpl::playBGM(const StringRef& filePath, float volume, float pitch, double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::stopBGM(double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::playBGS(const StringRef& filePath, float volume, float pitch, double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::stopBGS(double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::playME(const StringRef& filePath, float volume, float pitch)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::stopME()
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::playSE(const StringRef& filePath, float volume, float pitch)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::playSE3D(const StringRef& filePath, const Vector3& position, float distance, float volume, float pitch)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::stopSE()
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::setMEFadeState(double begin, double end)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::setBGMVolume(float volume, double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::setBGSVolume(float volume, double fadeTime)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::setEnableBGMRestart(bool enabled)
{
    LN_NOTIMPLEMENTED();
}

void GameAudioImpl::setEnableBGSRestart(bool enabled)
{
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln

