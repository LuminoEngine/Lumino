/*
	■参考
	Cri API
	http://www.criware.jp/adx2le/docs/windows/index_man.html

*/
#include "Internal.h"
#include "AudioDevice.h"
#ifdef LN_OS_WIN32
	#include "XAudio2/XAudio2AudioDevice.h"
	#include "DirectMusic/DirectMusicAudioDevice.h"
#endif
#include "NullAudioDevice.h"
#include <Lumino/Base/Environment.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/Audio/Sound.h>
#include <Lumino/Audio/GameAudio.h>
#include "AudioUtils.h"
#include "WaveDecoder.h"
#include "MidiDecoder.h"
#include "GameAudioImpl.h"
#include "AudioManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// AudioManagerImplImpl
//==============================================================================

static AudioManager* g_audioManager = nullptr;

//------------------------------------------------------------------------------
AudioManager* AudioManager::GetInstance(AudioManager* priority)
{
	return (priority != nullptr) ? priority : g_audioManager;
}

//------------------------------------------------------------------------------
AudioManager::AudioManager()
	: m_fileManager(NULL)
	, m_audioDevice(NULL)
	, m_midiAudioDevice(NULL)
	, m_gameAudio(NULL)
	, m_onMemoryLimitSize(100000)
	, m_resourceMutex()
	, m_audioStreamCache(NULL)
	, m_addingSoundList()
	, m_soundList()
	, m_soundListMutex()
	, m_endRequested()
	, m_pollingThread()
{
}

//------------------------------------------------------------------------------
AudioManager::~AudioManager()
{
}

//------------------------------------------------------------------------------
void AudioManager::Initialize(const Settings& settings)
{
	m_fileManager = settings.fileManager;

#ifdef LN_OS_WIN32
	if (m_audioDevice == NULL)
	{
		RefPtr<XAudio2AudioDevice> device(LN_NEW XAudio2AudioDevice(), false);
		if (device->Initialize()) {
			device.SafeAddRef();
			m_audioDevice = device;
		}
		//mAudioDevice = LN_NEW NullAudioDevice();
	}
	if (m_midiAudioDevice == NULL)
	{
		RefPtr<DirectMusicAudioDevice> device(LN_NEW DirectMusicAudioDevice(), false);
		DirectMusicAudioDevice::ConfigData data;
		data.DMInitMode = settings.directMusicInitMode;
		data.hWnd = (HWND)settings.hWnd;
		data.ReverbLevel = settings.directMusicReverbLevel;
		device->Initialize(data);
		device.SafeAddRef();
		m_midiAudioDevice = device;
	}
#else
#endif
	if (m_audioDevice == nullptr)
	{
		m_audioDevice = LN_NEW NullAudioDevice();
	}

	// キャッシュ初期化
	m_audioStreamCache = LN_NEW CacheManager(settings.streamCacheObjectCount, settings.streamSourceCacheMemorySize);

	// GameAudio
	m_gameAudio = LN_NEW GameAudioImpl(this);

	// ポーリングスレッド開始
	m_pollingThread.Start(Delegate<void()>(this, &AudioManager::Thread_Polling));

	if (g_audioManager == nullptr)
	{
		g_audioManager = this;
	}
}

//------------------------------------------------------------------------------
void AudioManager::Finalize()
{
	// ポーリングスレッドの終了を待つ
	m_endRequested.SetTrue();
	m_pollingThread.Wait();

	// GameAudio のデストラクタでは Sound::Stop が呼ばれるので、
	// これより下の Sound 削除処理の前に delete する。
	LN_SAFE_DELETE(m_gameAudio);

	// 何か残っていれば削除する
	for (Sound* sound : m_addingSoundList) {
		sound->Release();
	}
	m_addingSoundList.Clear();

	// 何か残っていれば削除する
	for (Sound* sound : m_soundList) {
		sound->Release();
	}
	m_soundList.Clear();


	if (m_audioStreamCache != NULL) {
		m_audioStreamCache->Finalize();
		LN_SAFE_RELEASE(m_audioStreamCache);
	}
	LN_SAFE_RELEASE(m_midiAudioDevice);
	LN_SAFE_RELEASE(m_audioDevice);

	if (g_audioManager == this)
	{
		g_audioManager = nullptr;
	}
}

//------------------------------------------------------------------------------
AudioStream* AudioManager::CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
{
	// キャッシュを検索する。
	// 見つかった AudioStream は、まだ非同期初期化中であることもある。
	RefPtr<AudioStream> audioStream((AudioStream*)m_audioStreamCache->FindObjectAddRef(key), false);

	// キャッシュに見つからなかったら新しく作る
	if (audioStream.IsNull())
	{
		audioStream.Attach(LN_NEW AudioStream(this, stream), false);
		audioStream->Create(loadingMode == SoundLoadingMode::ASync);	// 非同期読み込み開始
								// TODO: 同期読み込みだけにして、Polling スレッドで読み込んでも良いかも？
		/*
			非同期読み込みの開始で FileManager のタスクリストに入れられる。
			そこで参照カウントが +1 され、処理が完了するまで参照され続ける。
		*/

		// キャッシュに登録
		if (!key.IsNull()) {
			m_audioStreamCache->RegisterCacheObject(key, audioStream);
		}
	}

	audioStream.SafeAddRef();
	return audioStream;
}

//------------------------------------------------------------------------------
AudioPlayer* AudioManager::CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D)
{
	// 再生方法の選択
	SoundPlayingMode playerType = AudioUtils::CheckAudioPlayType(mode, stream, m_onMemoryLimitSize);

	// 作成
	if (playerType == SoundPlayingMode::Midi) {
		return m_midiAudioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
	else {
		return m_audioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
}

//------------------------------------------------------------------------------
Sound* AudioManager::CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
{
	RefPtr<AudioStream> audioStream(CreateAudioStream(stream, key, loadingMode), false);
	RefPtr<Sound> sound(LN_NEW Sound(), false);
	sound->Initialize(this, audioStream);

	if (loadingMode == SoundLoadingMode::Sync) {
		sound->CreateAudioPlayerSync();
	}

	// 管理リストに追加
	Threading::MutexScopedLock lock(m_soundListMutex);
	m_addingSoundList.Add(sound);
	sound.SafeAddRef();	// 管理リストの参照
	sound.SafeAddRef();	// 外に出すための参照
	return sound;
}

//------------------------------------------------------------------------------
void AudioManager::Thread_Polling()
{
#ifdef LN_OS_WIN32
	// COM 初期化
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif

	uint64_t lastTime = Environment::GetTickCount();
	while (m_endRequested.IsFalse())
	{
		Threading::Thread::Sleep(10);	// CPU 負荷 100% を避けるため、とりあえず 10ms 待つ

		// 追加待ちリストの内容を本リストに移す
		{
			Threading::MutexScopedLock lock(m_soundListMutex);
			for (Sound* sound : m_addingSoundList) {
				m_soundList.Add(sound);
			}
			m_addingSoundList.Clear();
		}

		// 経過時間を求めて全 Sound 更新
		uint64_t curTime = Environment::GetTickCount();
		float elapsedTime = static_cast<float>(curTime - lastTime) / 1000.0f;
		for (Sound* sound : m_soundList) {
			sound->Polling(elapsedTime);
		}
		lastTime = curTime;

#if 1	// 同時再整数が多くなると重くなるらしい問題の修正。とりあえずこれで様子を見る。
#else
		// ここからロック
		Threading::MutexScopedLock lock(m_soundListMutex);
#endif

		m_audioDevice->Update();
        
        if (m_midiAudioDevice != nullptr)
        {
            m_midiAudioDevice->Update();
        }

		// GC。このリストからしか参照されてなければ Release する。
		Array<Sound*>::iterator itr = m_soundList.begin();
		Array<Sound*>::iterator end = m_soundList.end();
		while (itr != end)
		{
			// TODO: フェード中は開放しない

			if ((*itr)->GetRefCount() == 1)
			{
				(*itr)->Release();
				itr = m_soundList.erase(itr);
				end = m_soundList.end();
			}
			else {
				++itr;
			}
		}

		if (m_gameAudio != NULL) {
			m_gameAudio->Polling();
		}
	}

#ifdef LN_OS_WIN32
	::CoUninitialize();
#endif
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
