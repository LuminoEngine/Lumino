#pragma once
#include <memory>
#include <thread>
#include <LuminoEngine/Audio/InternalSharedMutex.inc>
#include "../Base/RefObjectCache.hpp"
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
	//void createAudioDecoderAsync(const StringRef& filePath, std::function<void(AudioDecoder* decoder)>);
	void releaseAudioDecoder(AudioDecoder* decoder);

private:
	void processThread();
	void loadingThread();

    AssetManager* m_assetManager;
	Ref<AudioContext> m_primaryContext;
    Ref<GameAudioImpl> m_gameAudio;
	ObjectCache<String, AudioDecoder> m_decoderCache;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;
	AudioRWMutex m_cacheMutex;

	std::unique_ptr<std::thread> m_audioThread;
	std::unique_ptr<std::thread> m_loadingThread;
	std::unique_ptr<Exception> m_audioThreadException;
	std::atomic<bool> m_endRequested;
};

} // namespace detail
} // namespace ln

