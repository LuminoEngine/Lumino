
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/AudioGainNode.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "AudioDecoder.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// Sound

Sound::Sound()
    : m_gameAudioFlags(0)
    , m_fadeValue()
    , m_fadeBehavior(SoundFadeBehavior::Continue)
    , m_fading(false)
{
}

Sound::~Sound()
{
}

void Sound::init(const StringRef& filePath)
{
    Object::init();

    detail::AudioManager* manager = detail::EngineDomain::audioManager();

    Ref<detail::AudioDecoder> decoder = detail::EngineDomain::audioManager()->createAudioDecoder(filePath);
    m_sourceNode = newObject<AudioSourceNode>(decoder);
    m_gainNode = newObject<AudioGainNode>();


    //AudioNode::connect(m_sourceNode, manager->primaryContext()->destination());
    AudioNode::connect(m_sourceNode, m_gainNode);
    AudioNode::connect(m_gainNode, manager->primaryContext()->destination());

    //auto panner = newObject<AudioPannerNode>();
    //AudioNode::connect(source, panner);
    //AudioNode::connect(panner, AudioContext::primary()->destination());
    //source->setPlaybackRate(1.2);
    //source->start();
}

void Sound::onDispose(bool explicitDisposing)
{
    if (m_gainNode) {
        m_gainNode->dispose();
        m_sourceNode = nullptr;
    }
    if (m_sourceNode) {
        m_sourceNode->dispose();
        m_sourceNode = nullptr;
    }
    Object::onDispose(explicitDisposing);
}

void Sound::setVolume(float volume)
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);
    setVolumeInternal(volume);
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
    m_sourceNode->setLoop(enabled);
}

bool Sound::isLoopEnabled() const
{
    return m_sourceNode->loop();
}

void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
    LN_NOTIMPLEMENTED();
}

void Sound::play()
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);
    playInternal();
}

void Sound::stop()
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);
    stopInternal();
}

void Sound::pause()
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);
    pauseInternal();
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

SoundPlayingState Sound::playingState() const
{
    switch (m_sourceNode->playingState())
    {
    case AudioSourceNode::PlayingState::NoChanged:
    case AudioSourceNode::PlayingState::Stop:
        return SoundPlayingState::Stopped;
    case AudioSourceNode::PlayingState::Play:
        return SoundPlayingState::Playing;
    case AudioSourceNode::PlayingState::Pause:
        return SoundPlayingState::Pausing;
    default:
        LN_UNREACHABLE();
        return SoundPlayingState::Stopped;
    }
}

void Sound::fadeVolume(float targetVolume, double time, SoundFadeBehavior behavior)
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);

    // 現在の音量から targetVolume への遷移
    targetVolume = Math::clamp(targetVolume, 0.0f, 1.0f);
    m_fadeValue.start(getVolume(), targetVolume, time);
    m_fadeBehavior = behavior;
    m_fading = true;
}

void Sound::process(float elapsedSeconds)
{
    std::lock_guard<std::mutex> lock(m_playerStateLock);

    // フェード中の場合 (m_playerState の Volume,PlayingState 上書き)
    if (m_fading)
    {
        m_fadeValue.advanceTime(elapsedSeconds);
        setVolumeInternal(m_fadeValue.getValue());

        // フェード完了
        if (m_fadeValue.isFinished())
        {
            m_fading = false;

            // フェード終了時の動作によって分岐
            switch (m_fadeBehavior)
            {
                // 継続する場合
            case SoundFadeBehavior::Continue:
                break;
                // 停止する場合
            case SoundFadeBehavior::stop:
            case SoundFadeBehavior::StopReset:
                playInternal();
                break;
                // 一時停止する場合
            case SoundFadeBehavior::pause:
            case SoundFadeBehavior::PauseReset:
                pauseInternal();
                break;
            }

            // 音量を元に戻す
            if (m_fadeBehavior == SoundFadeBehavior::StopReset || SoundFadeBehavior::StopReset == SoundFadeBehavior::PauseReset)
            {
                setVolumeInternal(m_fadeValue.getStartValue());
            }
        }
    }
}

void Sound::playInternal()
{
    m_sourceNode->start();
}

void Sound::stopInternal()
{
    m_sourceNode->stop();
}

void Sound::pauseInternal()
{
    LN_NOTIMPLEMENTED();
}

void Sound::setVolumeInternal(float volume)
{
    m_gainNode->setGain(volume);
}

} // namespace ln

