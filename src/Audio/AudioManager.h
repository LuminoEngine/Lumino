
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class Sound;
class GameAudio;

namespace detail
{
class AudioDevice;
class AudioStream;
class AudioPlayer;
class GameAudioImpl;

// 音声機能の管理クラス
class AudioManager
	: public RefObject
{
public:

	struct Settings
	{
		ln::FileManager*	fileManager = nullptr;							// ファイルからの読み込みに使うファイル管理クラス
		uint32_t			streamCacheObjectCount = 32;					// キャッシュサイズ (ファイル数)
		uint32_t			streamSourceCacheMemorySize = 0;				// キャッシュサイズ (メモリ量(byte))
		DirectMusicMode		directMusicInitMode = DirectMusicMode::NotUse;	// DirectMusic の初期化方法
		void*				hWnd = nullptr;									// DirectMusic の初期化に使うウィンドウハンドル
		float				directMusicReverbLevel = 0.75f;
	};

public:
	static AudioManager* GetInstance(AudioManager* priority = nullptr);

	AudioManager();
	virtual ~AudioManager();
	void Initialize(const Settings& settings);
	void Finalize();

	/// オンメモリorストリーミング自動選択の音声データバイト数閾値
	void SetAutoPlayTypeSelectThreshold(uint32_t threshold) { m_onMemoryLimitSize = threshold; }

	FileManager* GetFileManager() { return m_fileManager; }
	GameAudioImpl* GetGameAudio() const { return m_gameAudio; }
	AudioDevice* GetAudioDevice() const { return m_audioDevice; }

	AudioStream* CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);
	AudioPlayer* CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D);		// 初期化完了済みの AudioStream を渡すこと
	Sound* CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);

private:
	void Thread_Polling();

	FileManager*				m_fileManager;
	AudioDevice*				m_audioDevice;		// PCM 再生用デバイスクラス
	AudioDevice*				m_midiAudioDevice;	// MIDI 再生用デバイスクラス (内部処理が PCM とは全然違うので、1つの AudioDevice にまとめない方が管理が楽)
	GameAudioImpl*				m_gameAudio;
	uint32_t					m_onMemoryLimitSize;
	Threading::Mutex			m_resourceMutex;

	CacheManager*				m_audioStreamCache;

	Array<Sound*>				m_addingSoundList;
	Array<Sound*>				m_soundList;
	Threading::Mutex			m_soundListMutex;
	Threading::EventFlag		m_endRequested;
	Threading::DelegateThread	m_pollingThread;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
