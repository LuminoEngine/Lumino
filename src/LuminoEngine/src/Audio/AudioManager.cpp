
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Audio/AudioContext.hpp>
#include "AudioDecoder.hpp"
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

void AudioManager::initialize(const Settings& settings)
{
	m_primaryContext = makeRef<AudioContext>();
	m_primaryContext->initialize();

	//m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	//m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);



#ifdef LN_AUDIO_THREAD_ENABLED
	m_endRequested = false;
	m_audioThread = std::make_unique<std::thread>(std::bind(&AudioManager::processThread, this));
#endif
}

void AudioManager::dispose()
{
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

void AudioManager::update()
{
	if (!m_audioThread) {
		// not thread processing.
		if (m_primaryContext) {
			m_primaryContext->process();
		}
	}
}

Ref<AudioDecoder> AudioManager::createAudioDecoder(const StringRef & filePath)
{
	// TODO: diag
	auto diag = newObject<DiagnosticsManager>();

	// TODO: cache
	auto decoder = makeRef<WaveDecoder>();
	decoder->initialize(FileStream::create(filePath), diag);
	return decoder;
}

void AudioManager::processThread()
{
	try
	{
		while (!m_endRequested)
		{
			if (m_primaryContext) {
				m_primaryContext->process();
			}
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

