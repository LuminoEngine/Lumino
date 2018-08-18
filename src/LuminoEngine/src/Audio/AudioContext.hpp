#pragma once

namespace ln {
namespace detail {
class AudioDevice;
class AudioDestinationNode;

class AudioContext
	: public RefObject
{
public:
	AudioContext();
	virtual ~AudioContext() = default;
	void initialize();
	virtual void dispose();
	void process();

private:
	Ref<AudioDevice> m_device;
	Ref<AudioDestinationNode> m_destinationNode;
};

} // namespace detail
} // namespace ln

