
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
#include "../../../Engine/src/Asset/AssetManager.hpp"
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
	m_gameAudio2 = makeRef<GameAudioImpl2>(this);

    LN_LOG_DEBUG << "AudioManager Initialization ended.";
}

void AudioManager::dispose()
{

	{
		auto disposeList = m_soundManagementList;
		for (auto& value : disposeList) {
			value->dispose();
		}
	}

	if (m_primaryContext) {
		m_primaryContext->removeAllNodes();
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

	if (m_gameAudio) {
		m_gameAudio->dispose();
		m_gameAudio = nullptr;
	}
	if (m_gameAudio2) {
		m_gameAudio2->dispose();
		m_gameAudio2 = nullptr;
	}
	
    if (m_dispatcher) {
        m_dispatcher->executeTasks();   // 残っているものを実行してしまう
        m_dispatcher = nullptr;
    }

	// 残っているコマンドを全て実行する
	commitCommands();
	updateSoundCores(1.0f);

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
		updateSoundCores(elapsedSeconds);
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

		if (filePath.endsWith(_TT(".wav"), CaseSensitivity::CaseInsensitive)) {
			auto d = makeRef<WaveDecoder>();
			d->init(stream, diag);
			decoder = d;
		}
		else if (filePath.endsWith(_TT(".ogg"), CaseSensitivity::CaseInsensitive)) {
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

//void AudioManager::createAudioDecoderAsync(const StringRef& filePath, const std::function<void(AudioDecoder* decoder)>& postAction)
//{
//    auto task = Task::create([this, filePath, postAction]() {
//        auto decoder = createAudioDecoder(filePath);
//        m_dispatcher->post([postAction, decoder]() {
//            postAction(decoder);
//        });
//    });
//    //task->awaitThen([]() {
//    //    postAction
//    //});
//}

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


void AudioManager::postAddAudioNode(AudioContext* targetContext, ARINode* node)
{
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::AddNode, targetContext, node, nullptr });
}

void AudioManager::postRemoveAudioNode(AudioContext* targetContext, ARINode* node)
{
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::AddNode, targetContext, node, nullptr });
}

void AudioManager::postConnect(ARINode* outputSide, ARINode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context())) return;
	if (LN_REQUIRE(inputSide->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::Connection, nullptr, outputSide, inputSide });
}

void AudioManager::postDisconnect(ARINode* outputSide, ARINode* inputSide)
{
	if (LN_REQUIRE(outputSide)) return;
	if (LN_REQUIRE(inputSide)) return;
	if (LN_REQUIRE(outputSide->context())) return;
	if (LN_REQUIRE(inputSide->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::Disconnection, nullptr, outputSide, inputSide });
}

void AudioManager::postDisconnectAll(ARINode* node)
{
	if (LN_REQUIRE(node)) return;
	if (LN_REQUIRE(node->context())) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::DisconnectionAll, nullptr, node, nullptr });
}

void AudioManager::postAddSoundCore(SoundCore* soundCore)
{
	if (LN_REQUIRE(soundCore)) return;
	std::lock_guard<std::mutex> lock(m_commandsMutex);
	m_commands.push_back({ OperationCode::AddSoundCore, nullptr, nullptr, nullptr, soundCore });
}

void AudioManager::commitCommands()
{
	std::lock_guard<std::mutex> lock(m_commandsMutex);

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
				node->disconnectAll();
				//if (node->frontNode()) {
				//	node->frontNode()->m_alived = false;
				//}
				//node->dispose();
				break;
			}
			case OperationCode::AddSoundCore:
				m_soundCoreList.add(cmd.soundCore);
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
		m_commands.clear();
	}
}

void AudioManager::updateSoundCores(float elapsedSeconds)
{
	for (int i = m_soundCoreList.size() - 1; i >= 0; i--) {
		const auto& sound = m_soundCoreList[i];
		sound->update(elapsedSeconds);


		const auto state = sound->lifecycleState.load();
		switch (state)
		{
			case SoundCoreLifecycleState::Valid:

				break;
			case SoundCoreLifecycleState::Disposed:
				sound->dispose();
				m_soundCoreList.removeAt(i);
				break;
			case SoundCoreLifecycleState::DisposeAfterStop:
				break;
			default:
				LN_UNREACHABLE();
				break;
		}
	}

	if (m_gameAudio2) {
		m_gameAudio2->update(elapsedSeconds);
	}
}

void AudioManager::processThread()
{
	LN_LOG_DEBUG << "Audio thread started.";

#ifdef _WIN32
	// NOTE: Chromium の REALTIME_AUDIO
	if (!::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)) {
		LN_LOG_WARNING << "SetThreadPriority Failed.";
	}
#else
	LN_LOG_WARNING << "Audio thread priority - not implemented.";
#endif
	try
	{
		// スレッド優先度の影響があるのか、GetTickCount() では正確な経過時間が取れないことがある。
		// (例えば Sleep(5) の前後で GetTickCount() が返す値が変わらない)
		// そのため高精度タイマーで計測する。
		ElapsedTimer timer;
		float elapsedSeconds = 0.01;
		const int64_t idleBaseTime = 5;
		while (!m_endRequested)
		{
			timer.start();

			const uint64_t processStartTime = timer.elapsedMilliseconds();
			{
				commitCommands();
				updateSoundCores(elapsedSeconds);
				if (m_primaryContext) {
					m_primaryContext->processOnAudioThread(elapsedSeconds);
				}
			}
			const uint64_t processEndTime = timer.elapsedMilliseconds();

			const int64_t frameElapsed = processEndTime - processStartTime;
			const int64_t sleepTime = std::min(idleBaseTime - frameElapsed, idleBaseTime);
			if (sleepTime > 0) {
				Thread::sleep(sleepTime);
			}

			elapsedSeconds = 0.001f * (timer.elapsedMilliseconds() - processStartTime);
		}
	}
	catch (Exception& e)
	{
		m_audioThreadException.reset(e.copy());
        LN_LOG_ERROR << m_audioThreadException->message();
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
		LN_LOG_ERROR << m_audioThreadException->message();
	}
}

} // namespace detail
} // namespace ln

