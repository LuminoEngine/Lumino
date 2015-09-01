/*
	DirectMusic は、単純に再生だけをするなら IDirectMusicAudioPath 等のインターフェイスに
	アクセスする必要は無く、このソースのように規模が大きくなることもない。
	パフォーマンスの初期化時にエフェクトありのユーティリティ設定を使用することで、
	最初からリバーブ有効で初期化もできる。

	ただ、エフェクトありで初期化すると何故かボリュームやピッチの動的な調整ができなくなる。
	そのため、エフェクトなしで初期化した後、自前でエフェクトの設定を行っている。

	ちなみに、リバーブのレベルは RGSS のよりも若干落として音割れを防いでいる。
*/
#include "../Internal.h"
#include <algorithm>
#include <cguid.h>
#include "DirectMusic.h"

#define LN_OUT_LOG  // 念のためログを出しておく

namespace Lumino
{
namespace Audio
{

//=============================================================================
// DirectMusicSegment 
//=============================================================================

#define LN_DMUS_TYPE DMUS_APATH_DYNAMIC_STEREO  // DMUS_APATH_SHARED_STEREOPLUSREVERB にするとデフォでリバーブ付くけど、ピッチ変更ができなくなるみたい

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
	DirectMusicSegment::DirectMusicSegment(IDirectMusicPerformance8* dmPerformance, MidiDecoder* midiStream)
	: m_dmPerformance(dmPerformance)
	, m_dmAudioPath(NULL)
	, m_dmSegment(NULL)
	, m_dmSegmentState(NULL)
	, m_dsSoundBuffer(NULL)
{
    if ( m_dmPerformance )
    {
		//-----------------------------------------------------
		// dmPerformance に対してエフェクトを設定する

        // DirectSound のバッファを取得するため、オーディオパスを取得する
		LN_COMCALL(m_dmPerformance->CreateStandardAudioPath(
			LN_DMUS_TYPE,		// パスのタイプ
			64,					// パス内のパフォーマンス チャンネルの数
			FALSE,				// 作成時にパスをアクティブにするか
			&m_dmAudioPath));
            
        // DirectSound バッファの取得
		LN_COMCALL(m_dmAudioPath->GetObjectInPath(
            DMUS_PCHANNEL_ALL,			// 検索するパフォーマンス チャンネル。DMUS_PCHANNEL_ALL を指定すると、すべてのチャンネルが検索される。
            DMUS_PATH_BUFFER,			// オーディオパス内のステージ。DMUS_PATH_BUFFER で DirectSound バッファを取得する。
            0,							// バッファのインデックス
			GUID_NULL,					// 検索するオブジェクトのクラス識別子。
			0,							// 一致するオブジェクトのリストにおけるオブジェクトのインデックス。
            IID_IDirectSoundBuffer8,	// 目的のインターフェイスの識別子。
            (void**)&m_dsSoundBuffer));

        // バッファにエフェクトを設定する
		//		GUID_DSFX_STANDARD_CHORUS
		//		GUID_DSFX_STANDARD_COMPRESSOR
		//		GUID_DSFX_STANDARD_DISTORTION
		//		GUID_DSFX_STANDARD_ECHO			… エコーありすぎ。かなり強くてつらい。
		//		GUID_DSFX_STANDARD_FLANGER
		//		GUID_DSFX_STANDARD_GARGLE		… ぶるぶる
		//		GUID_DSFX_STANDARD_I3DL2REVERB	… 音ちいさい
		//		GUID_DSFX_STANDARD_PARAMEQ
		//		GUID_DSFX_WAVES_REVERB			… RGSS 的リバーブ
		DSEFFECTDESC ds_desc;
		ZeroMemory(&ds_desc, sizeof(DSEFFECTDESC));
		ds_desc.dwSize			= sizeof(DSEFFECTDESC);
		ds_desc.dwFlags			= 0;//DSFX_LOCSOFTWARE;
		ds_desc.guidDSFXClass	= GUID_DSFX_WAVES_REVERB;  // エフェクト識別子 ( ウェーブリバーブ )
		ds_desc.dwReserved1		= 0;	// 予約済み
		ds_desc.dwReserved2		= 0;	// 予約済み
		LN_COMCALL(m_dsSoundBuffer->SetFX(1, &ds_desc, NULL));
		    
        // リバーブのパラメータを設定するためのインターフェースを取得する
		// ( 普通は必要ないけど、できるだけ RGSS に近付けたいので )
		IDirectSoundFXWavesReverb8* waves_reverb = NULL;
		LN_COMCALL(m_dsSoundBuffer->GetObjectInPath(GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8, (void**)&waves_reverb));
		    
		// パラメータを設定して バッファにセット
		DSFXWavesReverb rv;
		rv.fInGain			= 0.f;//DSFX_WAVESREVERB_INGAIN_MIN ;//DSFX_WAVESREVERB_INGAIN_DEFAULT;
		rv.fReverbMix		= 0.f;//DSFX_WAVESREVERB_REVERBMIX_MIN  DSFX_WAVESREVERB_REVERBMIX_DEFAULT;
		rv.fReverbTime		= 600.f;//DSFX_WAVESREVERB_REVERBTIME_DEFAULT;DSFX_WAVESREVERB_REVERBTIME_MIN 
		rv.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;//DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN; //DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX
		LN_COMCALL(waves_reverb->SetAllParameters(&rv));
		    
		//-----------------------------------------------------
		// セグメントを作成する

		//ローダーの作成
		IDirectMusicLoader8* loader = NULL;
		LN_COMCALL(CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&loader));
    		
		// キャッシュ無効 (有効にする場合、http://www.nomuraz.com/denpa/prog001.htm#IDPROG0009 のバグに注意)
		LN_COMCALL(loader->EnableCache(GUID_DirectMusicAllTypes, FALSE));

		// セグメント作成のための情報を設定する
		DMUS_OBJECTDESC desc;
		ZeroMemory( &desc, sizeof( DMUS_OBJECTDESC ) );
		desc.dwSize			= sizeof( DMUS_OBJECTDESC );
		desc.dwValidData	= DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
		desc.guidClass		= CLSID_DirectMusicSegment;
		desc.llMemLength	= (LONGLONG)midiStream->GetSourceDataSize();	// バッファのサイズ
		desc.pbMemData		= (LPBYTE)midiStream->GetSourceData();		  // データの入っているバッファ

		// セグメント作成
		LN_COMCALL(loader->GetObject(&desc, IID_IDirectMusicSegment8, (void**)&m_dmSegment));

        // SetParam で GUID_StandardMIDIFile を設定する前に呼ばないと
        // 第 1 トラックが再生されなくなる
		LN_COMCALL(m_dmSegment->Download(m_dmPerformance));

		// SMF として再生することを伝える
		LN_COMCALL(m_dmSegment->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL));
    	
        // ローダーはもう使わないので解放
		LN_SAFE_RELEASE( loader );
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DirectMusicSegment::~DirectMusicSegment()
{
	Stop();

	if (m_dmSegment) {
		m_dmSegment->Unload(m_dmPerformance);
	}
	if (m_dmPerformance) {
		m_dmPerformance->CloseDown();
	}

	LN_SAFE_RELEASE(m_dmSegmentState);
	LN_SAFE_RELEASE(m_dmSegment);
	LN_SAFE_RELEASE(m_dsSoundBuffer);
	LN_SAFE_RELEASE(m_dmAudioPath);
	LN_SAFE_RELEASE(m_dmPerformance);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicSegment::SetVolume(int volume)
{
	// 10db ～ -100db が実用範囲。渡す値は 1000 ～ -10000
	// デフォルトは 1db ( 100 ) で、これを超えると大体音割れする。(リバーブエフェクト)
	long v = 70 * (100 - static_cast< long >(volume)) / 100;
	v *= v;
	v = -v + 100;
	if (v > 100)
	{
		v = 100;
	}
	m_dmPerformance->SetGlobalParam(GUID_PerfMasterVolume, &v, sizeof(long));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicSegment::SetPitch(int pitch)
{
	DWORD p = 22050 * static_cast< DWORD >(pitch) / 100;
	HRESULT hr = m_dsSoundBuffer->SetFrequency(p);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t DirectMusicSegment::GetTotalTime() const
{
	MUSIC_TIME segment_length = 0;
	m_dmSegment->GetLength(&segment_length);
    return segment_length;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t DirectMusicSegment::GetPlayPosition() const
{
	MUSIC_TIME time = 0;
	if (m_dmSegmentState)
	{
		m_dmSegmentState->GetSeek(&time);
		return time;
	}
    return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicSegment::SetPlayPosition(uint32_t time)
{
	if (m_dmSegment)
	{
		m_dmSegment->SetStartPoint(time);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool DirectMusicSegment::IsPlaying() const
{
	return m_dmPerformance->IsPlaying(m_dmSegment, m_dmSegmentState) == S_OK;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DirectMusicSegment::SetLoopState(bool isLoop, uint32_t begin, uint32_t length)
{
	// ループ再生する場合
	if (isLoop)
	{
		if (length <= 0)
		{
			// セグメント全体の長さ取得
			MUSIC_TIME time_length;
			LN_COMCALL(m_dmSegment->GetLength(&time_length));
			length = time_length;

			LN_COMCALL(m_dmSegment->SetLoopPoints(begin, length));
		}
		else
		{
			LN_COMCALL(m_dmSegment->SetLoopPoints(begin, begin + length));
		}

		// 無限ループ
		m_dmSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
	}
	// ループ再生しない場合
	else
	{
		m_dmSegment->SetLoopPoints(0, 0);
		m_dmSegment->SetRepeats(0);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicSegment::Play()
{
	Stop();

	LN_SAFE_RELEASE(m_dmSegmentState);

	LN_COMCALL(m_dmAudioPath->Activate(TRUE));

	IDirectMusicSegmentState* state = NULL;
	LN_COMCALL(m_dmPerformance->PlaySegmentEx(m_dmSegment, NULL, NULL, 0, 0, &state, NULL, m_dmAudioPath));

	state->QueryInterface(IID_IDirectMusicSegmentState8, (LPVOID*)&m_dmSegmentState);
	state->Release();

	// 非同期で初期化してた場合、ボリュームなどは、再生開始直前に設定する必要がある
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicSegment::Stop()
{
	if (m_dmAudioPath != NULL)
	{
		LN_COMCALL(m_dmAudioPath->Activate(FALSE));
	}
	if (m_dmPerformance != NULL)
	{
		LN_COMCALL(m_dmPerformance->Stop(m_dmSegment, NULL, 0, 0));
	}
}


//=============================================================================
// DirectMusicSegment 
//=============================================================================

// PlayerObject クラスのデストラクタ
DirectMusicManager::PlayerObject::~PlayerObject()
{
    DirectMusicManager::GetInstance()->RemovePlayRequest( this );
}

DirectMusicManager* DirectMusicManager::m_instance = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::Initialize(const ConfigData& configData)
{
	if (!m_instance && configData.DMInitMode != DirectMusicInitMode_NotUse)
	{
		m_instance = LN_NEW DirectMusicManager();
		m_instance->InternalInitialize(configData);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::Finalize()
{
	LN_SAFE_DELETE(m_instance);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DirectMusicManager::DirectMusicManager()
	: m_windowHandle(NULL)
	, m_initMode(DirectMusicInitMode_ThreadWait)
	, m_directSound(NULL)
	, m_directMusic(NULL)
	, m_firstPerformance(NULL)
	, mErrorState(1)       // 最初はエラーにしておく。正常に初期化できたら 0
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DirectMusicManager::~DirectMusicManager()
{
	// スレッドの終了を待って解放
	m_initThread.Wait();

	LN_SAFE_RELEASE(m_firstPerformance);
	LN_SAFE_RELEASE(m_directSound);
	LN_SAFE_RELEASE(m_directMusic);

	//::CoUninitialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t DirectMusicManager::GetErrorState()
{
	Threading::MutexScopedLock lock(m_mutex);
    return mErrorState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::AddPlayRequest(PlayerObject* obj)
{
	Threading::MutexScopedLock lock(m_mutex);

	// 再生要求リストに追加されていない場合は追加しておく
	PlayRequestList::iterator pos;
	pos = std::find(m_playRequestList.begin(), m_playRequestList.end(), obj);
	if (pos == m_playRequestList.end())
	{
		m_playRequestList.Add(obj);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::RemovePlayRequest( PlayerObject* obj )
{
	Threading::MutexScopedLock lock(m_mutex);
	m_playRequestList.Remove(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IDirectMusicPerformance8* DirectMusicManager::CreateDMPerformance()
{
    // 一応初期化済みかをチェック
	if (!IsInitialized()) { return NULL; }

    m_mutex.Lock();
    IDirectMusicPerformance8* performance = m_firstPerformance;
    m_mutex.Unlock();

    // 一番最初に初期化したものがある場合はそれを返す
    if ( performance )
	{
		m_mutex.Lock();
		m_firstPerformance = NULL;
		m_mutex.Unlock();
        return performance;
    }

    // パフォーマンスの作成
    LN_COMCALL(CoCreateInstance(
		CLSID_DirectMusicPerformance,
		NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		(void**)&performance ));

    // パフォーマンスの初期化
	//hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
	LN_COMCALL(performance->InitAudio(
		(IDirectMusic**)&m_directMusic, (IDirectSound**)&m_directSound,
		m_windowHandle,
		LN_DMUS_TYPE,	
		64, DMUS_AUDIOF_ALL, NULL));

    return performance;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::Polling()
{
	if (m_performanceInited.IsTrue())
	{
		Threading::MutexScopedLock lock(m_mutex);

		if (m_playRequestList.GetCount() > 0)
		{
			PlayRequestList::iterator it = m_playRequestList.begin();
			PlayRequestList::iterator end = m_playRequestList.end();
			for (; it != end; ++it)
			{
				(*it)->onFinishDMInit(CreateDMPerformance());
			}
			m_playRequestList.Clear();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::InternalInitialize(const ConfigData& configData)
{
	LN_THROW(configData.WindowHandle, ArgumentException);

    m_windowHandle	= configData.WindowHandle;
    m_initMode		= configData.DMInitMode;

    // COM 初期化
    //HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
    //LN_SET_DXERR_R_S( hr, "< DirectMusicManager::_initialize >\nCoInitializeEx" );

    // DirectSound の作成
	LN_COMCALL(::CoCreateInstance(
		CLSID_DirectSound8,
		NULL, 
		CLSCTX_ALL,
		IID_IDirectSound8,
		(void**)&m_directSound));

    // DirectSound の初期化
	LN_COMCALL(m_directSound->Initialize(NULL));

	// 協調レベルの設定
	LN_COMCALL(m_directSound->SetCooperativeLevel(m_windowHandle, DSSCL_PRIORITY));

	// DirectMusic 作成
	LN_COMCALL(CoCreateInstance(CLSID_DirectMusic, NULL, CLSCTX_INPROC, IID_IDirectMusic, (void**)&m_directMusic));

	// 作成した DirectMusic に DirectSound を設定
	LN_COMCALL(m_directMusic->SetDirectSound(m_directSound, m_windowHandle));

	if (m_initMode == DirectMusicInitMode_ThreadWait || m_initMode == DirectMusicInitMode_ThreadRequest)
	{
		m_initThread.Start(LN_CreateDelegate(this, &DirectMusicManager::Thread_InitPerformance));
	}
	else
	{
		InitPerformance();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::InitPerformance()
{
#ifdef LN_OUT_LOG
	Logger::WriteLine("initialize IDirectMusicPerformance8 ...");
#endif

    IDirectMusicPerformance8* performance = NULL;
    uint32_t error = 0;
        
    // パフォーマンスの作成
    HRESULT hr = CoCreateInstance(
		CLSID_DirectMusicPerformance,
		NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		(void**)&performance );
    if ( FAILED( hr ) ) error = 1; 

    if ( SUCCEEDED( hr ) )
    {
        // パフォーマンスの初期化
		//hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		hr = performance->InitAudio(
			(IDirectMusic**)&m_directMusic, (IDirectSound**)&m_directSound,
			m_windowHandle,
			LN_DMUS_TYPE,
			64, DMUS_AUDIOF_ALL, NULL );
		if ( FAILED( hr ) ) error = 2;
    }

#ifdef LN_OUT_LOG
	Logger::WriteLine("lock IDirectMusicPerformance8");
#endif

    m_mutex.Lock();
    m_firstPerformance = performance;
    mErrorState = error;
    m_mutex.Unlock();

    // 初期化完了
    m_performanceInited.SetTrue();

#ifdef LN_OUT_LOG
	Logger::WriteLine("success initialize IDirectMusicPerformance8");
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DirectMusicManager::Thread_InitPerformance()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	DirectMusicManager::GetInstance()->InitPerformance();
	CoUninitialize();
}

} // namespace Audio
} // namespace Lumino
