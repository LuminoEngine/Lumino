#pragma once
#include "../Engine/RenderingCommandList.hpp"

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;

class AudioManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	AudioManager();
	virtual ~AudioManager();
	void initialize(const Settings& settings);
	void dispose();
	void update();

	const Ref<AudioContext>& primaryContext() const { return m_primaryContext; }
	//RenderingType renderingType() const { return RenderingType::Immediate; }
	//const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

	Ref<AudioDecoder> createAudioDecoder(const StringRef& filePath);

private:
	Ref<AudioContext> m_primaryContext;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;

#if LN_AUDIO_THREAD_ENABLED
	//std::unique_ptr<std::thread> m_audioThread;
#endif
};

} // namespace detail
} // namespace ln

