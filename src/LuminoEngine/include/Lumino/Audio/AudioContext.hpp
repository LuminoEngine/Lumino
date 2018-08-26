#pragma once
#include <shared_mutex>

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class CoreAudioDestinationNode;
} // namespace detail
class AudioNode;
class AudioDestinationNode;

class AudioContext
	: public RefObject
{
public:
	static AudioContext* primary();	// TODO: move Engine 

	AudioContext();
	virtual ~AudioContext() = default;
	void initialize();
	virtual void dispose();
	void process();

	AudioDestinationNode* destination() const;

	// TODO: internal
	detail::AudioManager* manager() const { return m_manager; }

	detail::AudioDevice* coreObject();

	void sendConnect(AudioNode* outputSide, AudioNode* inputSide);
	void sendDisconnect(AudioNode* outputSide, AudioNode* inputSide);
	void sendDisconnectAllAndDispose(AudioNode* node);

LN_INTERNAL_ACCESS:
#ifdef LN_AUDIO_THREAD_ENABLED
	std::shared_mutex m_commitMutex;
	std::shared_mutex& commitMutex() { return m_commitMutex; }
#endif

	void addAudioNode(AudioNode* node) { m_allAudioNodes.add(node); }
	void removeAudioNode(AudioNode* node) { m_allAudioNodes.remove(node); }

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
	void commitGraphs();

	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_audioDevice;
	Ref<detail::CoreAudioDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;

	List<AudioNode*> m_allAudioNodes;
	std::vector<ConnectionCommand> m_connectionCommands;
};

} // namespace ln

