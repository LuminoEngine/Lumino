
#include "Internal.h"
#ifdef LN_OS_WIN32
	#include "XAudio2/XAudio2AudioDevice.h"
	#include "DirectMusic/DirectMusicAudioDevice.h"
#endif
#include <Lumino/Base/Environment.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Audio/Sound.h>
#include "AudioUtils.h"
#include "WaveDecoder.h"
#include "MidiDecoder.h"

namespace Lumino
{
namespace Audio
{

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
// AudioManager
//=============================================================================

AudioManager* Internal::Manager = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManager* AudioManager::Create(const Settings& settings)
{
	return LN_NEW AudioManager(settings);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManager::AudioManager(const Settings& settings)
	: m_fileManager(settings.FileManager)
	, m_audioDevice(NULL)
	, m_midiAudioDevice(NULL)
	, mOnMemoryLimitSize(100000)
	, m_audioStreamCache(NULL)
{
#ifdef LN_OS_WIN32
	if (m_audioDevice == NULL)
	{
		RefPtr<XAudio2AudioDevice> device(LN_NEW XAudio2AudioDevice());
		if (device->Initialize()) {
			device.SafeAddRef();
			m_audioDevice = device;
		}
		//mAudioDevice = LN_NEW NullAudioDevice();
	}
	if (m_midiAudioDevice == NULL)
	{
		RefPtr<DirectMusicAudioDevice> device(LN_NEW DirectMusicAudioDevice());
		DirectMusicAudioDevice::ConfigData data;
		data.DMInitMode = settings.DMInitMode;
		data.hWnd = (HWND)settings.hWnd;
		device->Initialize(data);
		device.SafeAddRef();
		m_midiAudioDevice = device;
	}
#else
#endif
	// キャッシュ初期化
	m_audioStreamCache = LN_NEW CacheManager(32, 65535);

	// ポーリングスレッド開始
	m_pollingThread.Start(LN_CreateDelegate(this, &AudioManager::Thread_Polling));

	Internal::Manager = this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManager::~AudioManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioManager::Finalize()
{
	// ポーリングスレッドの終了を待つ
	m_endRequested.SetTrue();
	m_pollingThread.Wait();

	// 何か残っていれば削除する
	LN_FOREACH(Sound* sound, m_soundList) {
		sound->Release();
	}
	m_soundList.Clear();

	if (m_audioStreamCache != NULL) {
		m_audioStreamCache->Finalize();
		LN_SAFE_RELEASE(m_audioStreamCache);
	}
	LN_SAFE_RELEASE(m_audioDevice);
	LN_SAFE_RELEASE(m_midiAudioDevice);

	if (Internal::Manager == this) {
		Internal::Manager = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream* AudioManager::CreateAudioStream(const TCHAR* filePath)
{
	RefPtr<Stream> stream(m_fileManager->CreateFileStream(filePath));
	return CreateAudioStream(stream, CacheKey(PathName(filePath)));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream* AudioManager::CreateAudioStream(Stream* stream, const CacheKey& key)
{
	// キャッシュを検索する。
	// 見つかった AudioStream は、まだ非同期初期化中であることもある。
	RefPtr<AudioStream> audioStream((AudioStream*)m_audioStreamCache->FindObjectAddRef(key), false);

	// キャッシュに見つからなかったら新しく作る
	if (audioStream.IsNull())
	{
		audioStream.Attach(LN_NEW AudioStream(stream), false);
		audioStream->Create();	// 非同期読み込み開始
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
AudioPlayer* AudioManager::CreateAudioPlayer(AudioStream* stream, SoundPlayType type, bool enable3D)
{
	// 再生方法の選択
	SoundPlayType playerType = AudioUtils::CheckAudioPlayType(type, stream, mOnMemoryLimitSize);

	// 作成
	if (playerType == SoundPlayType_Midi) {
		return m_midiAudioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
	else {
		return m_audioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* AudioManager::CreateSound(Stream* stream, const CacheKey& key)
{
	RefPtr<AudioStream> audioStream(CreateAudioStream(stream, key));
	RefPtr<Sound> sound(LN_NEW Sound(this, audioStream));

	// 管理リストに追加
	Threading::MutexScopedLock lock(m_soundListMutex);
	m_soundList.Add(sound);
	sound.SafeAddRef();	// 管理リストの参照
	sound.SafeAddRef();	// 外に出すための参照
	return sound;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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

		// 経過時間を求めて全 Sound 更新
		float elapsedTime = static_cast<float>(Environment::GetTickCount() - lastTime) / 1000.0f;
		LN_FOREACH(Sound* sound, m_soundList)
		{
			sound->Polling(elapsedTime);
		}

		// ここからロック
		Threading::MutexScopedLock lock(m_soundListMutex);

		m_audioDevice->Update();
		m_midiAudioDevice->Update();

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
	}

#ifdef LN_OS_WIN32
	::CoUninitialize();
#endif
}

} // namespace Audio
} // namespace Lumino
