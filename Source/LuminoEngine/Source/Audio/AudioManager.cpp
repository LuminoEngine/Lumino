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
AudioManager* AudioManager::getInstance(AudioManager* priority)
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
void AudioManager::initialize(const Settings& settings)
{
	m_fileManager = settings.fileManager;

#ifdef LN_OS_WIN32
	if (m_audioDevice == NULL)
	{
		RefPtr<XAudio2AudioDevice> device(LN_NEW XAudio2AudioDevice(), false);
		if (device->initialize()) {
			device.safeAddRef();
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
		device->initialize(data);
		device.safeAddRef();
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
	m_pollingThread.start(Delegate<void()>(this, &AudioManager::thread_Polling));

	if (g_audioManager == nullptr)
	{
		g_audioManager = this;
	}
}

//------------------------------------------------------------------------------
void AudioManager::Finalize()
{
	// ポーリングスレッドの終了を待つ
	m_endRequested.setTrue();
	m_pollingThread.wait();

	// GameAudio のデストラクタでは Sound::stop が呼ばれるので、
	// これより下の Sound 削除処理の前に delete する。
	LN_SAFE_DELETE(m_gameAudio);

	// 何か残っていれば削除する
	for (Sound* sound : m_addingSoundList) {
		sound->release();
	}
	m_addingSoundList.clear();

	// 何か残っていれば削除する
	for (Sound* sound : m_soundList) {
		sound->release();
	}
	m_soundList.clear();


	if (m_audioStreamCache != NULL) {
		m_audioStreamCache->finalizeCache();
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
AudioStream* AudioManager::createAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
{
	// キャッシュを検索する。
	// 見つかった AudioStream は、まだ非同期初期化中であることもある。
	RefPtr<AudioStream> audioStream((AudioStream*)m_audioStreamCache->findObjectAddRef(key), false);

	// キャッシュに見つからなかったら新しく作る
	if (audioStream.isNull())
	{
		audioStream.attach(LN_NEW AudioStream(this, stream), false);
		audioStream->create(loadingMode == SoundLoadingMode::ASync);	// 非同期読み込み開始
								// TODO: 同期読み込みだけにして、polling スレッドで読み込んでも良いかも？
		/*
			非同期読み込みの開始で FileManager のタスクリストに入れられる。
			そこで参照カウントが +1 され、処理が完了するまで参照され続ける。
		*/

		// キャッシュに登録
		if (!key.isNull()) {
			m_audioStreamCache->registerCacheObject(key, audioStream);
		}
	}

	audioStream.safeAddRef();
	return audioStream;
}

//------------------------------------------------------------------------------
AudioPlayer* AudioManager::createAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D)
{
	// 再生方法の選択
	SoundPlayingMode playerType = AudioUtils::checkAudioPlayType(mode, stream, m_onMemoryLimitSize);

	// 作成
	if (playerType == SoundPlayingMode::Midi) {
		return m_midiAudioDevice->createAudioPlayer(stream, enable3D, playerType);
	}
	else {
		return m_audioDevice->createAudioPlayer(stream, enable3D, playerType);
	}
}

//------------------------------------------------------------------------------
void AudioManager::addSound(Sound* sound)
{
	// 管理リストに追加
	MutexScopedLock lock(m_soundListMutex);
	m_addingSoundList.add(sound);
	sound->addRef();//.SafeAddRef();	// 管理リストの参照
	//sound.SafeAddRef();	// 外に出すための参照
	//return sound;
}

//------------------------------------------------------------------------------
void AudioManager::thread_Polling()
{
#ifdef LN_OS_WIN32
	// COM 初期化
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif

	uint64_t lastTime = Environment::getTickCount();
	while (m_endRequested.isFalse())
	{
		Thread::sleep(10);	// CPU 負荷 100% を避けるため、とりあえず 10ms 待つ

		// 追加待ちリストの内容を本リストに移す
		{
			MutexScopedLock lock(m_soundListMutex);
			for (Sound* sound : m_addingSoundList) {
				m_soundList.add(sound);
			}
			m_addingSoundList.clear();
		}

		// 経過時間を求めて全 Sound 更新
		uint64_t curTime = Environment::getTickCount();
		float elapsedTime = static_cast<float>(curTime - lastTime) / 1000.0f;
		for (Sound* sound : m_soundList) {
			sound->polling(elapsedTime);
		}
		lastTime = curTime;

#if 1	// 同時再整数が多くなると重くなるらしい問題の修正。とりあえずこれで様子を見る。
#else
		// ここからロック
		MutexScopedLock lock(m_soundListMutex);
#endif

		m_audioDevice->update();
        
        if (m_midiAudioDevice != nullptr)
        {
            m_midiAudioDevice->update();
        }

		// GC。このリストからしか参照されてなければ Release する。
		List<Sound*>::iterator itr = m_soundList.begin();
		List<Sound*>::iterator end = m_soundList.end();
		while (itr != end)
		{
			// TODO: フェード中は開放しない

			if ((*itr)->getReferenceCount() == 1)
			{
				(*itr)->release();
				itr = m_soundList.erase(itr);
				end = m_soundList.end();
			}
			else {
				++itr;
			}
		}

		if (m_gameAudio != NULL) {
			m_gameAudio->polling();
		}
	}

#ifdef LN_OS_WIN32
	::CoUninitialize();
#endif
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
