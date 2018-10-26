#pragma once
#include <memory>
#include <thread>
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
	void processThread();

	Ref<AudioContext> m_primaryContext;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;

	std::unique_ptr<std::thread> m_audioThread;
	std::unique_ptr<Exception> m_audioThreadException;
	std::atomic<bool> m_endRequested;
};

} // namespace detail
} // namespace ln

