
#include "Internal.hpp"
#include "../Engine/LinearAllocator.hpp"
#include <Lumino/Engine/Diagnostics.hpp>
#include <Lumino/Audio/AudioContext.hpp>
#include "AudioDecoder.hpp"
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
	m_primaryContext = makeRef<AudioContext>();
	m_primaryContext->initialize();

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();
	m_primaryRenderingCommandList = makeRef<RenderingCommandList>(m_linearAllocatorPageManager);
}

void AudioManager::dispose()
{
	if (m_primaryContext) {
		m_primaryContext->dispose();
		m_primaryContext.reset();
	}
}

void AudioManager::update()
{
	m_primaryContext->process();
}

Ref<AudioDecoder> AudioManager::createAudioDecoder(const StringRef & filePath)
{
	// TODO: diag
	auto diag = newObject<DiagnosticsManager>();

	// TODO: cache
	auto decoder = makeRef<WaveDecoder>();
	decoder->initialize(FileStream::create(filePath), diag);
	return decoder;
}

} // namespace detail
} // namespace ln

