
#include "Internal.hpp"
#include "AudioNode.hpp"
#include "AudioContext.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioContext

AudioContext::AudioContext()
{
}

void AudioContext::initialize()
{
	m_destinationNode = newObject<AudioDestinationNode>();
}

void AudioContext::dispose()
{
}

void AudioContext::process()
{
}

} // namespace detail
} // namespace ln

