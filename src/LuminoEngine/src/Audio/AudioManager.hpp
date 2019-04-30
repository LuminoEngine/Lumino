#pragma once
#include <memory>
#include <thread>
#include <LuminoEngine/Base/Task.hpp>
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
	void createAudioDecoderAsync(const StringRef& filePath, const std::function<void(AudioDecoder* decoder)>& postAction);
	void releaseAudioDecoder(AudioDecoder* decoder);

private:
    // processThread は少しでも遅れると音声に影響するので、できる限り Mixing に集中する。
    // 音声データの非同期ロードなどそれ以外は dispatheThread で行う。
	void processThread();
	void dispatheThread();

    AssetManager* m_assetManager;
	Ref<AudioContext> m_primaryContext;
    Ref<GameAudioImpl> m_gameAudio;
	ObjectCache<String, AudioDecoder> m_decoderCache;
	//Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	//Ref<RenderingCommandList> m_primaryRenderingCommandList;
	AudioRWMutex m_cacheMutex;
    Ref<Dispatcher> m_dispatcher;

	std::unique_ptr<std::thread> m_audioThread;
	std::unique_ptr<std::thread> m_dispatheThread;
	std::unique_ptr<Exception> m_audioThreadException;
	std::atomic<bool> m_endRequested;
};

} // namespace detail
} // namespace ln

