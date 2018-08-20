#pragma once

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class AudioContextCore;
class CoreAudioDestinationNode;
} // namespace detail
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

private:
	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_device;
	Ref<detail::AudioContextCore> m_audioContextHandler;
	Ref<detail::CoreAudioDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;
};

} // namespace ln

