
#include "Internal.hpp"
#include "AudioContext.hpp"
#include "AudioManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AudioManager

AudioManager::AudioManager()
{
}

void AudioManager::initialize(const Settings& settings)
{
	m_audioContext = makeRef<AudioContext>();
}

void AudioManager::dispose()
{
	if (m_audioContext) {
		m_audioContext->dispose();
		m_audioContext.reset();
	}
}

void AudioManager::update()
{
	m_audioContext->process();
}

} // namespace detail
} // namespace ln

