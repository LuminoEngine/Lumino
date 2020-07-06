#pragma once
#include <memory>
#include <thread>
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Audio/InternalSharedMutex.inc>
#include "../Base/RefObjectCache.hpp"
#include <LuminoEngine/Engine/RenderingCommandList.hpp>

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;
class AssetManager;
class GameAudioImpl;


//enum class OperationCode
//{
//	AddNode,
//	RemoveNode,
//	Connection,
//	Disconnection,
//	//DisconnectionAllAndDispose,
//	DisconnectionAll,
//};
//
//struct ConnectionCommand
//{
//	OperationCode code;
//	Ref<AudioNode> outputSide;
//	Ref<AudioNode> inputSide;
//};

class AudioManager
	: public RefObject
{
public:
	struct Settings
	{
        AssetManager* assetManager;
	};

	AudioManager();
	virtual ~AudioManager();
	void init(const Settings& settings);
	void dispose();
	void update(float elapsedSeconds);

	const Ref<AudioContext>& primaryContext() const { return m_primaryContext; }
    const Ref<GameAudioImpl>& gameAudio() const { return m_gameAudio; }
	//RenderingType renderingType() const { return RenderingType::Immediate; }
	//const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

	//// AudioNode インスタンス作成&
	//void addAudioNode(AudioNode* node);
	//void removeAudioNode(AudioNode* node);

	Ref<AudioDecoder> createAudioDecoder(const StringRef& filePath);
	void createAudioDecoderAsync(const StringRef& filePath, const std::function<void(AudioDecoder* decoder)>& postAction);
	void releaseAudioDecoder(AudioDecoder* decoder);

	void addSoundManagement(Sound* value);
	void removeSoundManagement(Sound* value);

	//void postAddCommand();

	void postAddAudioNode(AudioContext* targetContext, AudioNode* node);
	void postRemoveAudioNode(AudioContext* targetContext, AudioNode* node);
	void postConnect(AudioNode* outputSide, AudioNode* inputSide);
	void postDisconnect(AudioNode* outputSide, AudioNode* inputSide);
	//void sendDisconnectAllAndDispose(AudioNode* node);
	void postDisconnectAll(AudioNode* node);

private:
	enum class OperationCode
	{
		AddNode,
		RemoveNode,
		Connection,
		Disconnection,
		//DisconnectionAllAndDispose,
		DisconnectionAll,
	};

	struct ConnectionCommand
	{
		OperationCode code;
		Ref<AudioContext> context;
		Ref<detail::AudioNodeCore> outputSide;
		Ref<detail::AudioNodeCore> inputSide;
	};

	void commitCommands();
    // processThread は少しでも遅れると音声に影響するので、できる限り Mixing に集中する。
    // 音声データの非同期ロードなどそれ以外は dispatheThread で行う。
	void processThread();
	void dispatheThread();

    AssetManager* m_assetManager;
	Ref<AudioContext> m_primaryContext;
    Ref<GameAudioImpl> m_gameAudio;
	List<Sound*> m_soundManagementList;
	ObjectCache<String, AudioDecoder> m_decoderCache;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;
	AudioRWMutex m_cacheMutex;
    Ref<Dispatcher> m_dispatcher;

	std::unique_ptr<std::thread> m_audioThread;
	std::unique_ptr<std::thread> m_dispatheThread;
	std::unique_ptr<Exception> m_audioThreadException;
	std::atomic<bool> m_endRequested;
	std::vector<ConnectionCommand> m_commands;
	std::mutex m_commandsMutex;
};

} // namespace detail
} // namespace ln

