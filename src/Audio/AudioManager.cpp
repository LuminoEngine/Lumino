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

//class ASyncAudioStreamLoadTask
//	: public AsyncIOTask
//{
//public:
//	RefPtr<AudioStream>	m_audioStream;
//	RefPtr<Stream>		m_sourceStream;
//
//public:
//	virtual void Execute()
//	{
//		try
//		{
//			m_audioStream->Create(m_sourceStream);
//		}
//		catch (Exception* e) {
//			m_audioStream->OnCreateFinished(e);
//		}
//	}
//};
//
//=============================================================================
// AudioManagerImplImpl
//=============================================================================

AudioManagerImpl* Internal::AudioManager = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl* AudioManagerImpl::GetInstance(AudioManagerImpl* priority)
{
	if (priority != nullptr)
		return priority;
	return Internal::AudioManager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl* AudioManagerImpl::Create(const Settings& settings)
{
	return LN_NEW AudioManagerImpl(settings);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl::AudioManagerImpl(const Settings& settings)
	: m_fileManager(NULL)
	, m_audioDevice(NULL)
	, m_midiAudioDevice(NULL)
	, m_gameAudio(NULL)
	, mOnMemoryLimitSize(100000)
	, m_resourceMutex()
	, m_audioStreamCache(NULL)
	, m_addingSoundList()
	, m_soundList()
	, m_soundListMutex()
	, m_endRequested()
	, m_pollingThread()
{
	m_fileManager = settings.FileManager;

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
		data.DMInitMode = settings.DMInitMode;
		data.hWnd = (HWND)settings.hWnd;
		data.ReverbLevel = settings.DirectMusicReverbLevel;
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
	m_audioStreamCache = LN_NEW CacheManager(settings.StreamCacheObjectCount, settings.StreamSourceCacheMemorySize);

	// GameAudio
	m_gameAudio = LN_NEW GameAudioImpl(this);

	// ポーリングスレッド開始
	m_pollingThread.Start(Delegate<void()>(this, &AudioManagerImpl::Thread_Polling));

	Internal::AudioManager = this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl::~AudioManagerImpl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioManagerImpl::Finalize()
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

	if (Internal::AudioManager == this) {
		Internal::AudioManager = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream* AudioManagerImpl::CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer* AudioManagerImpl::CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D)
{
	// 再生方法の選択
	SoundPlayingMode playerType = AudioUtils::CheckAudioPlayType(mode, stream, mOnMemoryLimitSize);

	// 作成
	if (playerType == SoundPlayingMode::Midi) {
		return m_midiAudioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
	else {
		return m_audioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* AudioManagerImpl::CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioManagerImpl::Thread_Polling()
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

		// ここからロック
		Threading::MutexScopedLock lock(m_soundListMutex);

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

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
