#pragma once
#include "InternalSharedMutex.inc"

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class CoreAudioDestinationNode;
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
	void process(float elapsedSeconds);

    /** この AudioContext 内でベースとして使われるサンプルレート(1秒あたりのサンプル数)を取得します。 */
    int sampleRate() const;

	AudioDestinationNode* destination() const;

	// TODO: internal
	detail::AudioManager* manager() const { return m_manager; }

	detail::AudioDevice* coreObject();

	void sendConnect(AudioNode* outputSide, AudioNode* inputSide);
	void sendDisconnect(AudioNode* outputSide, AudioNode* inputSide);
	void sendDisconnectAllAndDispose(AudioNode* node);

    void addSound(Sound* sound);

LN_INTERNAL_ACCESS:
	detail::AudioRWMutex m_commitMutex;
	detail::AudioRWMutex& commitMutex() { return m_commitMutex; }

	void addAudioNode(AudioNode* node);
	void disposeNodeOnGenericThread(AudioNode* node);	// Audio Thread 以外での AudioNode::dispose

private:
	enum class OperationCode
	{
		Connection,
		Disconnection,
		DisconnectionAllAndDispose,
	};

	struct ConnectionCommand
	{
		OperationCode code;
		Ref<AudioNode> outputSide;
		Ref<AudioNode> inputSide;
	};

	// call by aduio thread.
	void commitGraphs(float elapsedSeconds);

	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_audioDevice;
	Ref<detail::CoreAudioDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;

	List<AudioNode*> m_allAudioNodes;
	std::vector<ConnectionCommand> m_connectionCommands;

	// commitGraphs() 中、m_allAudioNodes の AudioNode のインスタンスが消えないように参照を持っておくための list
	//List<Ref<AudioNode>> m_allAudioNodes_onCommit;

    List<WeakRefPtr<Sound>> m_soundList;
};

} // namespace ln

