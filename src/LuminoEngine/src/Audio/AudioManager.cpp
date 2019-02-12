
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

	m_decoderCache.init();
    m_dispatcher = makeRef<Dispatcher>();

	m_primaryContext = makeRef<AudioContext>();
	m_primaryContext->init();

	//m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	//m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);

#ifdef LN_AUDIO_THREAD_ENABLED
	m_endRequested = false;
	m_audioThread = std::make_unique<std::thread>(std::bind(&AudioManager::processThread, this));
    m_dispatheThread = std::make_unique<std::thread>(std::bind(&AudioManager::dispatheThread, this));
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
	if (m_dispatheThread) {
        m_dispatheThread->join();
	}
	if (m_audioThread) {
		m_audioThread->join();
	}
	if (m_audioThreadException) {
		throw *m_audioThreadException;
	}
#endif
    if (m_dispatcher) {
        m_dispatcher->executeTasks();   // 残っているものを実行してしまう
        m_dispatcher = nullptr;
    }

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
        m_dispatcher->executeTasks(1);
	}
}

Ref<AudioDecoder> AudioManager::createAudioDecoder(const StringRef& filePath)
{
	// TODO: diag
	auto diag = newObject<DiagnosticsManager>();

	auto path = Path(filePath).unify();
	Ref<AudioDecoder> decoder;
	{
		ScopedReadLock lock(m_cacheMutex);
		decoder = m_decoderCache.findObject(path);
	}

	if (decoder)
	{
		return decoder;
	}
	else
	{
		auto stream = m_assetManager->openFileStream(path);

		if (filePath.endsWith(u".wav", CaseSensitivity::CaseInsensitive)) {
			auto d = makeRef<WaveDecoder>();
			d->init(stream, diag);
			decoder = d;
		}
		else if (filePath.endsWith(u".ogg", CaseSensitivity::CaseInsensitive)) {
			auto d = makeRef<OggAudioDecoder>();
			d->init(stream, diag);
			decoder = d;
		}
		else {
			LN_ERROR("Invalid file extentsion.");
			return nullptr;
		}

		m_decoderCache.registerObject(path, decoder);

		return decoder;
	}
}

void AudioManager::createAudioDecoderAsync(const StringRef& filePath, const std::function<void(AudioDecoder* decoder)>& postAction)
{
    //auto task = Task::create([this, filePath]() {
    //    createAudioDecoder(filePath);
    //});
    //task->awaitThen([]() {
    //    postAction
    //});
}

void AudioManager::releaseAudioDecoder(AudioDecoder* decoder)
{
	ScopedWriteLock lock(m_cacheMutex);

	//if ()

	m_decoderCache.releaseObject(decoder);
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

void AudioManager::dispatheThread()
{
	try
	{
		while (!m_endRequested)
		{
            m_dispatcher->executeTasks(1);
			Thread::sleep(5);
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

