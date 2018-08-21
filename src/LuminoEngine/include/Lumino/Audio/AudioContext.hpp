#pragma once
#include <shared_mutex>

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class AudioContextCore;
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

	detail::AudioContextCore* coreObject();

LN_INTERNAL_ACCESS:
#if LN_AUDIO_THREAD_ENABLED
	std::shared_mutex commitMutex;
#endif

	void addAudioNode(AudioNode* node) { m_allAudioNodes.add(node); }
	void removeAudioNode(AudioNode* node) { m_allAudioNodes.remove(node); }

private:
	void commitGraphs();

	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_device;
	Ref<detail::AudioContextCore> m_audioContextHandler;
	Ref<detail::CoreAudioDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;

	List<AudioNode*> m_allAudioNodes;
};

} // namespace ln

