
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
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
	m_audioContext->initialize();

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);
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

