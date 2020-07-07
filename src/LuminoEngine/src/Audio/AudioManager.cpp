
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
#include "../Asset/AssetManager.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Audio/AudioContext.hpp>
#include <LuminoEngine/Audio/AudioNode.hpp>
#include <LuminoEngine/Audio/Sound.hpp>
#include "Decoder/WaveAudioDecoder.hpp"
#include "Decoder/OggAudioDecoder.hpp"
#include "ARIs/ARINode.hpp"
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
    LN_LOG_DEBUG << "AudioManager Initialization started.";

    m_assetManager = settings.assetManager;

	m_decoderCache.init();
    m_dispatcher = makeRef<Dispatcher>();

	m_primaryContext = makeRef<AudioContext>();
	m_primaryContext->init();

	//m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	//m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);

#ifdef LN_AUDIO_THREAD_ENABLED
	LN_LOG_DEBUG << "Audio thread enabled.";
	m_endRequested = false;
	m_audioThread = std::make_unique<std::thread>(std::bind(&AudioManager::processThread, this));
    m_dispatheThread = std::make_unique<std::thread>(std::bind(&AudioManager::dispatheThread, this));
#endif

    m_gameAudio = makeRef<GameAudioImpl>(this);

    LN_LOG_DEBUG << "AudioManager Initialization ended.";
}

void AudioManager::dispose()
{
    if (m_gameAudio) {
        m_gameAudio->dispose();
        m_gameAudio = nullptr;
    }

	{
		auto disposeList = m_soundManagementList;
		for (auto& value : disposeList) {
			value->dispose();
		}
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
	if (m_primaryContext) {
		m_primaryContext->updateFrame();
	}

	if (!m_audioThread) {
		// not thread processing.
		commitCommands();
		if (m_primaryContext) {
			m_primaryContext->processOnAudioThread(elapsedSeconds);
		}
        m_dispatcher->executeTasks(1);
	}
}

Ref<AudioDecoder> AudioManager::createAudioDecoder(const StringRef& filePath)
{
	// TODO: diag
	auto diag = makeObject<DiagnosticsManager>();

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
    auto task = Task::create([this, filePath, postAction]() {
        auto decoder = createAudioDecoder(filePath);
        m_dispatcher->post([postAction, decoder]() {
            postAction(decoder);
        });
    });
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

void AudioManager::addSoundManagement(Sound* value)
{
	if (LN_REQUIRE(value)) return;
	m_soundManagementList.add(value);
}

void AudioManager::removeSoundManagement(Sound* value)
{
	if (LN_REQUIRE(value)) return;
	m_soundManagementList.remove(value);
}


void AudioManager::postAddAudioNode(AudioContext* targetContext, AudioNode* node)
{
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::AddNode, targetContext, node->coreNode(), nullptr });
}

void AudioManager::postRemoveAudioNode(AudioContext* targetContext, AudioNode* node)
{
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::AddNode, targetContext, node->coreNode(), nullptr });
}

void AudioManager::postConnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context())) return;
	if (LN_REQUIRE(inputSide->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::Connection, nullptr, outputSide->coreNode(), inputSide->coreNode() });
}

void AudioManager::postDisconnect(AudioNode* outputSide, AudioNode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context())) return;
	if (LN_REQUIRE(inputSide->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::Disconnection, nullptr, outputSide->coreNode(), inputSide->coreNode() });
}

void AudioManager::postDisconnectAll(AudioNode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::DisconnectionAll, nullptr, node->coreNode(), nullptr });
}

void AudioManager::commitCommands()
{

	if (!m_commands.empty()) {
		for (const auto& cmd : m_commands) {
			switch (cmd.code) {
			case OperationCode::AddNode:
				cmd.context->addAudioNodeInternal(cmd.outputSide);
				break;
			case OperationCode::RemoveNode:
				cmd.context->removeAudioNodeInternal(cmd.outputSide);
				break;

			case OperationCode::Connection:
				//cmd.outputSide->m_alived = true;
				//cmd.inputSide->m_alived = true;
				detail::ARINode::connect(cmd.outputSide, cmd.inputSide);
				break;
			case OperationCode::Disconnection:
				LN_NOTIMPLEMENTED();
				break;
			case OperationCode::DisconnectionAll://AndDispose:
			{
				detail::ARINode* node = cmd.outputSide;
				node->disconnectAllInputSide();
				node->disconnectAllOutputSide();
				//if (node->frontNode()) {
				//	node->frontNode()->m_alived = false;
				//}
				//node->dispose();
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
			}
		}
		m_commands.clear();
	}

}

void AudioManager::processThread()
{
	LN_LOG_DEBUG << "Audio thread started.";
	try
	{
		while (!m_endRequested)
		{
			commitCommands();

			if (m_primaryContext) {
				m_primaryContext->processOnAudioThread(0.02);
			}

            Thread::sleep(20);
		}
	}
	catch (Exception& e)
	{
		m_audioThreadException.reset(e.copy());
        LN_LOG_ERROR << m_audioThreadException->getMessage();
	}
	LN_LOG_DEBUG << "Audio thread ended.";
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

