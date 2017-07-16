
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
	static AudioManager* getInstance(AudioManager* priority = nullptr);

	AudioManager();
	virtual ~AudioManager();
	void initialize(const Settings& settings);
	void dispose();

	/// オンメモリorストリーミング自動選択の音声データバイト数閾値
	void setAutoPlayTypeSelectThreshold(uint32_t threshold) { m_onMemoryLimitSize = threshold; }

	FileManager* getFileManager() { return m_fileManager; }
	GameAudioImpl* getGameAudio() const { return m_gameAudio; }
	AudioDevice* getAudioDevice() const { return m_audioDevice; }

	AudioStream* createAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);
	AudioPlayer* createAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D);		// 初期化完了済みの AudioStream を渡すこと
	//Sound* createSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);
	void addSound(Sound* sound);

private:
	void thread_Polling();

	FileManager*		m_fileManager;
	AudioDevice*		m_audioDevice;		// PCM 再生用デバイスクラス
	AudioDevice*		m_midiAudioDevice;	// MIDI 再生用デバイスクラス (内部処理が PCM とは全然違うので、1つの AudioDevice にまとめない方が管理が楽)
	GameAudioImpl*		m_gameAudio;
	uint32_t			m_onMemoryLimitSize;
	Mutex				m_resourceMutex;

	CacheManager*		m_audioStreamCache;

	List<Sound*>		m_addingSoundList;
	List<Sound*>		m_soundList;
	Mutex				m_soundListMutex;
	ConditionFlag		m_endRequested;
	DelegateThread		m_pollingThread;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
