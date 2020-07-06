#pragma once
#include "InternalSharedMutex.inc"

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class CoreAudioDestinationNode;
class AudioNodeCore;
} // namespace detail
class AudioNode;
class AudioDestinationNode;
class Sound;

class AudioContext
	: public RefObject
{
public:
	static AudioContext* primary();	// TODO: move Engine 

	AudioContext();
	virtual ~AudioContext() = default;
	void init();
	virtual void dispose();
	void updateFrame();

	void processOnAudioThread(float elapsedSeconds);

    /** この AudioContext 内でベースとして使われるサンプルレート(1秒あたりのサンプル数)を取得します。 */
    int sampleRate() const;

	AudioDestinationNode* destination() const;

	void addAudioNode(AudioNode* node);
	void removeAudioNode(AudioNode* node);

	// TODO: internal
	detail::AudioManager* manager() const { return m_manager; }
	//void markGC(detail::AudioNodeCore* node);	// call only audio thread.

	detail::AudioDevice* coreObject();

	void sendConnect(AudioNode* outputSide, AudioNode* inputSide);
	void sendDisconnect(AudioNode* outputSide, AudioNode* inputSide);
	//void sendDisconnectAllAndDispose(AudioNode* node);
	void sendDisconnectAll(AudioNode* node);

    void addSound(Sound* sound);

LN_INTERNAL_ACCESS:
	detail::AudioRWMutex m_commitMutex;
	detail::AudioRWMutex& commitMutex() { return m_commitMutex; }

	//void disposeNodeOnGenericThread(AudioNode* node);	// Audio Thread 以外での AudioNode::dispose
	//void tryRemoveAudioNode(AudioNode* node);

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
		Ref<detail::AudioNodeCore> outputSide;
		Ref<detail::AudioNodeCore> inputSide;
	};


	// call by aduio thread.
	void commitGraphs(float elapsedSeconds);

	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_audioDevice;
	Ref<detail::CoreAudioDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;

	// 強参照。ユーザープログラムからの明示的な破棄が必要。
	// Non-ThreadSafe.
	List<Ref<AudioNode>> m_audioNodes;

	// 強参照。AudioThread で受信したコマンドによって add/remove
	List<Ref<detail::AudioNodeCore>> m_coreAudioNodes;


	//List<AudioNode*> m_allAudioNodes;
	//List<Ref<AudioNode>> m_aliveNodes;
	//List<AudioNode*> m_markedNodes;

	// commitGraphs() 中、m_allAudioNodes の AudioNode のインスタンスが消えないように参照を持っておくための list
	//List<Ref<AudioNode>> m_allAudioNodes_onCommit;

	// TODO: Manager に持って行った方がいい。後々複数 context とかなった時にそれが必要。
	std::vector<ConnectionCommand> m_commands;

    List<WeakRefPtr<Sound>> m_soundList;
};

} // namespace ln

