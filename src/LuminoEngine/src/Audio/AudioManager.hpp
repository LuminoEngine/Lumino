#pragma once
#include <memory>
#include <thread>
#include "../Engine/RenderingCommandList.hpp"

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;
class AssetManager;
class GameAudioImpl;

class AudioManager
	: public RefObject
{
public:
	struct Settings
	{
        AssetManager* assetManager;
	};

	AudioManager();
	virtual ~AudioManager();
	void init(const Settings& settings);
	void dispose();
	void update(float elapsedSeconds);

	const Ref<AudioContext>& primaryContext() const { return m_primaryContext; }
    const Ref<GameAudioImpl>& gameAudio() const { return m_gameAudio; }
	//RenderingType renderingType() const { return RenderingType::Immediate; }
	//const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

	Ref<AudioDecoder> createAudioDecoder(const StringRef& filePath);

private:
	void processThread();

    AssetManager* m_assetManager;
	Ref<AudioContext> m_primaryContext;
    Ref<GameAudioImpl> m_gameAudio;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;

	std::unique_ptr<std::thread> m_audioThread;
	std::unique_ptr<Exception> m_audioThreadException;
	std::atomic<bool> m_endRequested;
};

} // namespace detail
} // namespace ln

