
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
#include "../Asset/AssetManager.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Audio/AudioContext.hpp>
#include "AudioDecoder.hpp"
#include "OggAudioDecoder.hpp"
#include "GameAudioImpl.hpp"
#include "AudioManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioManager

AudioManager::AudioManager()
	: m_primaryContext()
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::init(const Settings& settings)
{
    m_assetManager = settings.assetManager;
	m_primaryContext = makeRef<AudioContext>();
	m_primaryContext->init();

	//m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	//m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);

#ifdef LN_AUDIO_THREAD_ENABLED
	m_endRequested = false;
	m_audioThread = std::make_unique<std::thread>(std::bind(&AudioManager::processThread, this));
#endif

    m_gameAudio = makeRef<GameAudioImpl>(this);
}

void AudioManager::dispose()
{
    if (m_gameAudio) {
        m_gameAudio->dispose();
        m_gameAudio = nullptr;
    }

#ifdef LN_AUDIO_THREAD_ENABLED
	m_endRequested = true;

	if (m_audioThread) {
		m_audioThread->join();
	}
	if (m_audioThreadException) {
		throw *m_audioThreadException;
	}
#endif
	if (m_primaryContext) {
		m_primaryContext->dispose();
		m_primaryContext = nullptr;
	}
}

void AudioManager::update(float elapsedSeconds)
{
	if (!m_audioThread) {
		// not thread processing.
		if (m_primaryContext) {
			m_primaryContext->process(elapsedSeconds);
		}
	}
}

Ref<AudioDecoder> AudioManager::createAudioDecoder(const StringRef & filePath)
{
	// TODO: diag
	auto diag = newObject<DiagnosticsManager>();

    auto stream = m_assetManager->openFileStream(filePath);

	// TODO: cache
	auto decoder = makeRef<WaveDecoder>();
    //auto decoder = makeRef<OggAudioDecoder>();
	decoder->init(stream, diag);
	return decoder;
}

void AudioManager::processThread()
{
	try
	{
		while (!m_endRequested)
		{
			if (m_primaryContext) {
				m_primaryContext->process(0.02);
			}

            Thread::sleep(20);
		}
	}
	catch (Exception& e)
	{
		m_audioThreadException.reset(e.copy());
        LN_LOG_ERROR << m_audioThreadException->getMessage();
	}
}

} // namespace detail
} // namespace ln

