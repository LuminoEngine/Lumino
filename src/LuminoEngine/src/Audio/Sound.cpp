
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// Sound

Sound::Sound()
{
}

Sound::~Sound()
{
}

void Sound::initialize(const StringRef& filePath)
{
    Object::initialize();

    detail::AudioManager* manager = detail::EngineDomain::audioManager();

    m_sourceNode = newObject<AudioSourceNode>(filePath);
    AudioNode::connect(m_sourceNode, manager->primaryContext()->destination());


    //auto panner = newObject<AudioPannerNode>();
    //AudioNode::connect(source, panner);
    //AudioNode::connect(panner, AudioContext::primary()->destination());
    //source->setPlaybackRate(1.2);
    //source->start();
}

void Sound::setVolume(float volume)
{
    LN_NOTIMPLEMENTED();
}

float Sound::getVolume() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void Sound::setPitch(float pitch)
{
    m_sourceNode->setPlaybackRate(pitch);
}

float Sound::getPitch() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void Sound::setLoopEnabled(bool enabled)
{
    LN_NOTIMPLEMENTED();
}

bool Sound::isLoopEnabled() const
{
    LN_NOTIMPLEMENTED();
    return false;
}

void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
    LN_NOTIMPLEMENTED();
}

void Sound::play()
{
    m_sourceNode->start();
}

void Sound::stop()
{
    LN_NOTIMPLEMENTED();
}

void Sound::pause()
{
    LN_NOTIMPLEMENTED();
}

void Sound::resume()
{
    LN_NOTIMPLEMENTED();
}

void Sound::set3DEnabled(bool enabled)
{
    LN_NOTIMPLEMENTED();
}

bool Sound::is3DEnabled() const
{
    LN_NOTIMPLEMENTED();
    return false;
}

void Sound::setEmitterPosition(const Vector3& position)
{
    LN_NOTIMPLEMENTED();
}

const Vector3& Sound::getEmitterPosition() const
{
    LN_NOTIMPLEMENTED();
    return Vector3::Zero;
}

void Sound::setEmitterVelocity(const Vector3& velocity)
{
    LN_NOTIMPLEMENTED();
}

const Vector3& Sound::getEmitterVelocity() const
{
    LN_NOTIMPLEMENTED();
    return Vector3::Zero;
}

void Sound::setEmitterMaxDistance(float distance)
{
    LN_NOTIMPLEMENTED();
}

int64_t Sound::getTotalSamples() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

int64_t Sound::getPlayedSamples() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

int Sound::getSamplingRate() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

} // namespace ln

