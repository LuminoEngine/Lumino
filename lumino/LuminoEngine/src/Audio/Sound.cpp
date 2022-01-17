
#include "Internal.hpp"
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/AudioSourceNode.hpp>
#include <LuminoEngine/Audio/AudioGainNode.hpp>
#include <LuminoEngine/Audio/AudioDestinationNode.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "Decoder/AudioDecoder.hpp"
#include "ARIs/ARISourceNode.hpp"
#include "ARIs/ARIGainNode.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// Sound

Sound::Sound()
    : m_manager(nullptr)
	//, m_gameAudioFlags(0)
 //   , m_fadeValue()
 //   , m_fadeBehavior(SoundFadeBehavior::Continue)
 //   , m_fading(false)
{
}

Sound::~Sound()
{
}

void Sound::init(const StringView& filePath)
{
    Object::init();

	m_manager = detail::EngineDomain::audioManager();
	m_manager->addSoundManagement(this);

    AudioContext* context = m_manager->primaryContext();

    Ref<detail::AudioDecoder> decoder = detail::EngineDomain::audioManager()->createAudioDecoder(filePath);
   
    m_core = makeRef<detail::SoundCore>();
    if (!m_core->init(m_manager, context, decoder)) {
        LN_ERROR();
        return;
    }
    m_manager->postAddSoundCore(m_core);

    //m_core->m_context = context;

    //m_core->m_sourceNode = makeRef<detail::ARISourceNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), nullptr);
    //m_core->m_sourceNode->init(decoder);
    //m_manager->postAddAudioNode(context, m_core->m_sourceNode);

    //m_core->m_gainNode = makeRef<detail::ARIGainNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), nullptr);
    //m_core->m_gainNode->init();
    //m_manager->postAddAudioNode(context, m_core->m_gainNode);



    ////m_sourceNode = makeObject<AudioSourceNode>(decoder);
    ////m_manager->primaryContext()->addAudioNode(m_sourceNode);

    ////m_gainNode = makeObject<AudioGainNode>();
    ////m_manager->primaryContext()->addAudioNode(m_gainNode);

    //m_manager->postConnect(m_core->m_sourceNode, m_core->m_gainNode);
    //m_manager->postConnect(m_core->m_gainNode, m_manager->primaryContext()->destination()->coreNode());
    //m_manager->postAddSoundCore(m_core);

    //AudioNode::connect(m_sourceNode, manager->primaryContext()->destination());
    //AudioNode::connect(m_sourceNode, m_gainNode);
    //AudioNode::connect(m_gainNode, m_manager->primaryContext()->destination());

    //auto panner = makeObject<AudioPannerNode>();
    //AudioNode::connect(source, panner);
    //AudioNode::connect(panner, AudioContext::primary()->destination());
    //source->setPlaybackRate(1.2);
    //source->start();
}

void Sound::onDispose(bool explicitDisposing)
{
  //  if (m_gainNode) {
  //      m_gainNode->disconnect();
		//m_gainNode = nullptr;
  //  }
  //  if (m_sourceNode) {
  //      m_sourceNode->disconnect();
  //      m_sourceNode = nullptr;
  //  }
    if (m_core) {
        m_core->lifecycleState = detail::SoundCoreLifecycleState::Disposed;
    }
	if (m_manager) {
		m_manager->removeSoundManagement(this);
		m_manager = nullptr;
	}
    Object::onDispose(explicitDisposing);
}

void Sound::setVolume(float value)
{
    m_core->setVolume(value, 0, SoundFadeBehavior::Continue);
    //std::lock_guard<std::mutex> lock(m_playerStateLock);
    //setVolumeInternal(volume);
}

float Sound::getVolume() const
{
    return m_core->volume();
    //return m_gainNode->gain();
}

void Sound::setPitch(float value)
{
    m_core->setPitch(value);
    //m_sourceNode->setPlaybackRate(pitch);
}

float Sound::getPitch() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void Sound::setLoopEnabled(bool enabled)
{
    m_core->setLoopEnabled(enabled);
}

bool Sound::isLoopEnabled() const
{
    return m_core->m_sourceNode->staging.loop;
    //return m_sourceNode->loop();
}

void Sound::SetLoopRange(uint32_t begin, uint32_t length)
{
    LN_NOTIMPLEMENTED();
}

void Sound::play()
{
    m_core->play();
    //std::lock_guard<std::mutex> lock(m_playerStateLock);
    //playInternal();
}

void Sound::stop()
{
    m_core->stop(0.0f);
    //std::lock_guard<std::mutex> lock(m_playerStateLock);
    //stopInternal();
}

void Sound::pause()
{
    m_core->pause();
    //std::lock_guard<std::mutex> lock(m_playerStateLock);
    //pauseInternal();
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
    // TODO: 要求中の state も考慮する

    switch (m_core->m_sourceNode->playingState())
    {
    //case ln::detail::ARISourceNode::PlayingState::None:
    case ln::detail::ARISourceNode::State::Stopped:
        return SoundPlayingState::Stopped;
    case ln::detail::ARISourceNode::State::Playing:
        return SoundPlayingState::Playing;
    case ln::detail::ARISourceNode::State::Pausing:
        return SoundPlayingState::Pausing;
    default:
        LN_UNREACHABLE();
        return SoundPlayingState::Stopped;
    }
}

void Sound::fadeVolume(float targetVolume, float time, SoundFadeBehavior behavior)
{
    m_core->setVolume(targetVolume, time, behavior);
    //std::lock_guard<std::mutex> lock(m_playerStateLock);

    //// 現在の音量から targetVolume への遷移
    //targetVolume = Math::clamp(targetVolume, 0.0f, 1.0f);
    //m_fadeValue.start(getVolume(), targetVolume, time);
    //m_fadeBehavior = behavior;
    //m_fading = true;
}


//void Sound::playInternal()
//{
//    m_sourceNode->start();
//}
//
//void Sound::stopInternal()
//{
//    m_sourceNode->stop();
//}
//
//void Sound::pauseInternal()
//{
//    LN_NOTIMPLEMENTED();
//}
//
//void Sound::setVolumeInternal(float volume)
//{
//    m_gainNode->setGain(volume);
//}
//

//==============================================================================
// SoundCore

namespace detail {

SoundCore::SoundCore()
    : m_gameAudioFlags(0)
    , m_fadeValue()
    , lifecycleState(SoundCoreLifecycleState::Valid)
    , m_fadeBehavior(SoundFadeBehavior::Continue)
    , m_fading(false)
{
}

bool SoundCore::init(AudioManager* manager, AudioContext* context, detail::AudioDecoder* decoder)
{
    m_sourceNode = makeRef<detail::ARISourceNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), nullptr);
    m_sourceNode->init(decoder);
    manager->postAddAudioNode(context, m_sourceNode);

    m_gainNode = makeRef<detail::ARIGainNode>(detail::EngineDomain::audioManager()->primaryContext()->coreObject(), nullptr);
    m_gainNode->init();
    manager->postAddAudioNode(context, m_gainNode);



    //m_sourceNode = makeObject<AudioSourceNode>(decoder);
    //m_manager->primaryContext()->addAudioNode(m_sourceNode);

    //m_gainNode = makeObject<AudioGainNode>();
    //m_manager->primaryContext()->addAudioNode(m_gainNode);

    manager->postConnect(m_sourceNode, m_gainNode);
    manager->postConnect(m_gainNode, manager->primaryContext()->destination()->coreNode());

    return true;
}

void SoundCore::dispose()
{
    if (m_context) {
        m_gainNode->disconnectAll();
        m_sourceNode->disconnectAll();
        m_context->removeAudioNodeInternal(m_gainNode);
        m_context->removeAudioNodeInternal(m_sourceNode);
        m_gainNode = nullptr;
        m_sourceNode = nullptr;
        m_context = nullptr;
    }
}

void SoundCore::setVolume(float value, float fadeTime, SoundFadeBehavior behavior)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (fadeTime > 0.0f) {
        fadeVolume(value, fadeTime, behavior);
    }
    else {
        m_gainNode->staging.gain = value;
    }
}

float SoundCore::volume() const
{
    return m_gainNode->staging.gain.load();
}

void SoundCore::setPitch(float value)
{
    m_sourceNode->staging.playbackRate = value;
}

void SoundCore::setLoopEnabled(bool value)
{
    m_sourceNode->staging.loop = value;
}

void SoundCore::play()
{
    m_sourceNode->start();
}

void SoundCore::stop(float fadeTime)
{
    if (fadeTime > 0.0f) {
        fadeVolume(0.0f, fadeTime, SoundFadeBehavior::StopReset);
    }
    else {
        m_sourceNode->stop();
    }
}

void SoundCore::pause()
{
    LN_NOTIMPLEMENTED();
}

void SoundCore::fadeVolume(float targetVolume, float fadeTime, SoundFadeBehavior behavior)
{
    // 現在の音量から targetVolume への遷移
    m_fadeValue.start(volume(), Math::clamp(targetVolume, 0.0f, 1.0f), fadeTime);
    m_fadeBehavior = behavior;
    m_fading = true;
}

void SoundCore::update(float elapsedSeconds)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // フェード中の場合 (m_playerState の Volume,PlayingState 上書き)
    if (m_fading)
    {
        m_fadeValue.advanceTime(elapsedSeconds);
        m_gainNode->staging.gain = m_fadeValue.value();

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
            case SoundFadeBehavior::Stop:
            case SoundFadeBehavior::StopReset:
                m_sourceNode->stop();
                break;
                // 一時停止する場合
            case SoundFadeBehavior::Pause:
            case SoundFadeBehavior::PauseReset:
                pause();
                break;
            }

            // 音量を元に戻す
            if (m_fadeBehavior == SoundFadeBehavior::StopReset || SoundFadeBehavior::StopReset == SoundFadeBehavior::PauseReset)
            {
                m_gainNode->staging.gain = m_fadeValue.startValue();
            }
        }
    }
}

} // namespace detail
} // namespace ln

