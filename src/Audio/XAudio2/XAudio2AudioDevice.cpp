
#include "../Internal.h"
//#include "../../../Resource/LNResource.h"
//#include "../../../Math/LMath.h"
#include "XAudio2AudioPlayer.h"
#include "XAudio2AudioDevice.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// XAudio2AudioPlayerBase 
//=============================================================================

// 背面の音源が減衰されるコーンの設定
static const X3DAUDIO_CONE sListener_DirectionalCone = {
    X3DAUDIO_PI*5.0f/6.0f,
    X3DAUDIO_PI*11.0f/6.0f,
    1.0f, 1.75f,
    0.0f, 0.25f,
    0.708f, 1.0f };

// 音が完全に聞こえなくなる距離 (XACT 使えばこんなのいらないっぽいけど、なんか面倒なので)
static const float OuterDistance    = 14.0f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2AudioDevice::XAudio2AudioDevice()
    : mXAudio                   ( NULL )
    , mMasteringVoice           ( NULL )
    , mDeviceChannels           ( 0 )
    , mX3DAudioModule           ( NULL )
    , mMD_X3DAudioCalculate     ( NULL )
    , mMetreUnitDistanceInv     ( 1.0f )
    // , mDirectMusicAudioDevice   ( NULL )
    , mCoInited                 ( false )
{
    memset( mX3DInstance, 0, sizeof( mX3DInstance ) );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XAudio2AudioDevice::~XAudio2AudioDevice()
{
    //SAFE_DELETE( mDirectMusicAudioDevice );

    if ( mMasteringVoice )
	{
		mMasteringVoice->DestroyVoice();
		mMasteringVoice = NULL;
	}

    if ( mXAudio )
    {
        mXAudio->StopEngine();
		LN_SAFE_RELEASE( mXAudio );
    }

    if ( mCoInited )
    {
        ::CoUninitialize();
        mCoInited = false;
    }

    if ( mX3DAudioModule )
    {
        ::FreeLibrary( mX3DAudioModule );
        mX3DAudioModule = NULL;
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool XAudio2AudioDevice::Initialize(/* const ConfigData& configData */)
{
    if ( SUCCEEDED( ::CoInitializeEx( NULL, COINIT_MULTITHREADED ) ) )
    {
        mCoInited = true;
    }
        
    mX3DAudioModule = ::LoadLibrary( _T( "X3DAudio1_7.dll" ) );
	if ( mX3DAudioModule == NULL ) {
		return false;
	}

    UINT32 flags = 0;

  //  if ( configData.EnableDebugFlag )
  //  {
		//flags |= XAUDIO2_DEBUG_ENGINE;
  //  }

    // XAudio2 作成
    HRESULT hr = XAudio2Create( &mXAudio, flags );
	if ( FAILED( hr ) ) {
		return false;
	}

	// マスターボイス作成
	hr = mXAudio->CreateMasteringVoice( &mMasteringVoice );
	if ( FAILED( hr ) ) {
		return false;
	}

    Logger::WriteLine("get XAudio2AudioDevice information..." );

	// デバイス詳細情報の確認
	XAUDIO2_DEVICE_DETAILS details;
	hr = mXAudio->GetDeviceDetails( 0, &details );
    if ( SUCCEEDED( hr ) )
    {
		Logger::WriteLine(L"    DeviceID    : %s", details.DeviceID);
		Logger::WriteLine(L"    DisplayName : %s", details.DisplayName);
		Logger::WriteLine(L"    Role        : %d", details.Role);
		Logger::WriteLine(L"    OutputFormat ( WAVEFORMATEX )");
		Logger::WriteLine("        Format ID       : %hu", details.OutputFormat.Format.wFormatTag);
		Logger::WriteLine("        Channels        : %hu", details.OutputFormat.Format.nChannels);
		Logger::WriteLine("        SamplesPerSec   : %lu", details.OutputFormat.Format.nSamplesPerSec);
		Logger::WriteLine("        AvgBytesPerSec  : %lu", details.OutputFormat.Format.nAvgBytesPerSec);
		Logger::WriteLine("        BlockAlign      : %hu", details.OutputFormat.Format.nBlockAlign);
		Logger::WriteLine("        BitsPerSample   : %hu", details.OutputFormat.Format.wBitsPerSample);
		Logger::WriteLine("        ExtraSize       : %hu", details.OutputFormat.Format.cbSize);
    }
    else
    {
		Logger::WriteLine("failed to get information.");
    }

    // チャンネル数記憶
    mDeviceChannels = details.OutputFormat.Format.nChannels;
		
    // 関数アドレスを取得して 3D オーディオを初期化 ( グローバル情報の作成 )
    typedef void ( STDAPIVCALLTYPE *DEF_X3DAudioInitialize ) ( UINT32 , FLOAT32 , __out X3DAUDIO_HANDLE );
    DEF_X3DAudioInitialize md_X3DAudioInitialize = (DEF_X3DAudioInitialize)GetProcAddress( mX3DAudioModule, "X3DAudioInitialize" );

	md_X3DAudioInitialize(
		details.OutputFormat.dwChannelMask,		// Xbox 360 では SPEAKER_XBOX
		X3DAUDIO_SPEED_OF_SOUND, mX3DInstance );
		//SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER |
        //SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT

    // リスナーの設定
    mListenerState.OrientFront	= D3DXVECTOR3( 0, 0, 1 );	// 前方向の向き
	mListenerState.OrientTop	= D3DXVECTOR3( 0, 1, 0 );	// 上方向の向き ( OrientFront と正規直交であること )
	mListenerState.Position		= D3DXVECTOR3( 0, 0, 0 );
	mListenerState.Velocity		= D3DXVECTOR3( 0, 0, 0 );
	mListenerState.pCone		= NULL;//(X3DAUDIO_CONE*)&sListener_DirectionalCone;

    // X3DAudioCalculate() の関数アドレス
    mMD_X3DAudioCalculate = (DEF_X3DAudioCalculate)GetProcAddress( mX3DAudioModule, "X3DAudioCalculate" );

    // DirectMusic を初期化する場合
    //if ( configData.DMInitMode != DMINITMODE_NOTUSE && configData.Window )
    //{
    //    DirectMusic::XAudio2AudioDevice::ConfigData data;
    //    data.DMInitMode     = configData.DMInitMode;
    //    data.Window         = configData.Window;
    //    mDirectMusicAudioDevice = LN_NEW DirectMusic::XAudio2AudioDevice();
    //    mDirectMusicAudioDevice->initialize( data );
    //}

    return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioDevice::CalcEmitterState(EmitterState* emitter)
{
    if ( emitter )
    {
        static const DWORD calc_flags =
			X3DAUDIO_CALCULATE_MATRIX       |
            X3DAUDIO_CALCULATE_LPF_DIRECT   |
            X3DAUDIO_CALCULATE_LPF_REVERB   |
            X3DAUDIO_CALCULATE_REVERB       |
            X3DAUDIO_CALCULATE_DOPPLER;
            //X3DAUDIO_CALCULATE_EMITTER_ANGLE;
        //if (g_audioState.fUseRedirectToLFE)
		//{
		//	// On devices with an LFE channel, allow the mono source data
		//	// to be routed to the LFE destination channel.
		//	calc_flags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
		//}

        emitter->UpdateXAudioEmitter( mMetreUnitDistanceInv );
        emitter->DSPSettings.DstChannelCount = mDeviceChannels;

        mMD_X3DAudioCalculate(
            mX3DInstance,
            &mListenerState,
            &emitter->Emitter,
            calc_flags,
            &emitter->DSPSettings );
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer* XAudio2AudioDevice::CreateAudioPlayer(AudioStream* audioStream, bool enable3d, SoundPlayType type)
{
	RefPtr<AudioPlayer> audioPlayer;

    // 種類に応じてプレイヤーを作成する
	switch ( type )
	{
		// オンメモリ再生
		case SoundPlayType_OnMemory:
        {
			XAudio2OnMemoryAudioPlayer* player = LN_NEW XAudio2OnMemoryAudioPlayer(this);
			audioPlayer.Attach(player, false);
			player->Initialize(audioStream, enable3d);
			break;
        }
		// ストリーミング再生
		case SoundPlayType_Streaming:
        {
			XAudio2StreamingAudioPlayer* player = LN_NEW XAudio2StreamingAudioPlayer(this);
			audioPlayer.Attach(player, false);
			player->Initialize(audioStream, enable3d);
			break;
        }
		default:
			LN_THROW(0, ArgumentException);
		// SMF
		//case SOUNDPLAYTYPE_MIDI:
  //      {
		//	LN_THROW_InvalidOperation(mDirectMusicAudioDevice, Resource::String::ERR_DirectMusicNotInit);
  //          if ( mDirectMusicAudioDevice )
  //          {
		//		audio_player.attach(
		//			mDirectMusicAudioDevice->createAudioPlayer( source, enable_3d, type ) );
  //          }
		//	break;
  //      }
	}

	audioPlayer.SafeAddRef();
	return audioPlayer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioDevice::Update()
{
    mListenerState.OrientFront.x = m_soundListener.Direction.X;
    mListenerState.OrientFront.y = m_soundListener.Direction.Y;
    mListenerState.OrientFront.z = m_soundListener.Direction.Z;
    mListenerState.OrientTop.x = m_soundListener.UpDirection.X;
    mListenerState.OrientTop.y = m_soundListener.UpDirection.Y;
    mListenerState.OrientTop.z = m_soundListener.UpDirection.Z;
    mListenerState.Position.x = m_soundListener.Position.X * mMetreUnitDistanceInv;
    mListenerState.Position.y = m_soundListener.Position.Y * mMetreUnitDistanceInv;
    mListenerState.Position.z = m_soundListener.Position.Z * mMetreUnitDistanceInv;
    mListenerState.Velocity.x = m_soundListener.Velocity.X * mMetreUnitDistanceInv;
    mListenerState.Velocity.y = m_soundListener.Velocity.Y * mMetreUnitDistanceInv;
    mListenerState.Velocity.z = m_soundListener.Velocity.Z * mMetreUnitDistanceInv;

    //if ( mDirectMusicAudioDevice ) mDirectMusicAudioDevice->update();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XAudio2AudioDevice::SetListenerState(const Vector3& position, const Vector3& front)
{
    // 念のために正規化してから設定する
	Vector3 n = Vector3::Normalize(front);

	mListenerState.OrientFront.x = n.X;
	mListenerState.OrientFront.y = n.Y;
	mListenerState.OrientFront.z = n.Z;
	mListenerState.Position.x = position.X;
	mListenerState.Position.y = position.Y;
	mListenerState.Position.z = position.Z;
}

//=============================================================================
// EmitterState 
//=============================================================================

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[ 3 ]		= { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[ 3 ]	= { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EmitterState::EmitterState(uint32_t input_channels)//, lnU32 output_channels_ )
{
        
    EmitterAzimuths = LN_NEW FLOAT32[ input_channels ];
	MatrixCoefficients = LN_NEW FLOAT32[ input_channels * OUTPUTCHANNELS ];

	for (uint32_t i = 0; i < input_channels; ++i)
	{
		EmitterAzimuths[ i ] = 0;
	}

	// サウンドコーンの設定
	EmitterCone.InnerAngle		= 0.0f;		// 内部コーンの角度 ( ラジアン単位 0.0f ～ X3DAUDIO_2PI )
	EmitterCone.OuterAngle		= 0.0f;		// 外部コーンの角度 ( ラジアン単位 0.0f ～ X3DAUDIO_2PI )
	EmitterCone.InnerVolume	= 0.0f;		// 内部コーン上/内のボリュームスケーラ ( 0.0f ～ 2.0f )
	EmitterCone.OuterVolume	= 1.0f;		// 外部コーン上/以降のボリュームスケーラ ( 0.0f ～ 2.0f )
	EmitterCone.InnerLPF		= 0.0f;		// 内部コーン上/内の LPF ダイレクトパスまたはリバーブパスの係数スケーラ ( 0.0f ～ 1.0f )
	EmitterCone.OuterLPF		= 1.0f;		// 外部コーン上/以降の LPF ダイレクトパスまたはリバーブパスの係数スケーラ ( 0.0f ～ 1.0f )
	EmitterCone.InnerReverb	= 0.0f;		// 内部コーン上/内のリバーブ センドレベルスケーラ ( 0.0f ～ 2.0f )
	EmitterCone.OuterReverb	= 1.0f;		// 外部コーン上/以降のリバーブ センドレベルスケーラ ( 0.0f ～ 2.0f )
	
	// エミッタの設定
	Emitter.pCone			= &EmitterCone;			// サウンド コーンへのポインタまたは NULL ( NULL は全方向性 )
	Emitter.OrientFront	    = D3DXVECTOR3( 0, 0, 1 );	// 前方向の向き ( OrientTop と正規直交 )
	Emitter.OrientTop		= D3DXVECTOR3( 0, 1, 0 );	// 上方向の向き ( OrientFront と正規直交 )
	Emitter.Position		= D3DXVECTOR3( 0, 0, 0 );	// ワールド位置
	Emitter.Velocity		= D3DXVECTOR3( 0, 0, 0 );	// ユーザー定義のワールド単位/秒の速度

	Emitter.ChannelCount		= input_channels;		// X3DAUDIO_EMITTER 構造体によって定義されるエミッタの数
	Emitter.ChannelRadius		= 1.0f;					// ChannelCount が 1 より大きい場合にエミッタが配置される Position からの距離 ( 0.0f 以上 )
	Emitter.pChannelAzimuths	= EmitterAzimuths;		// ラジアン単位の方位角で表したチャンネル位置テーブル ( FLOAT32 の配列。各要素 0.0f ～ X3DAUDIO_2PI )
	Emitter.InnerRadius		    = 2.0f;					// 内部半径の計算に使用される値 ( 0.0f ～ MAX_FLT )
	Emitter.InnerRadiusAngle	= X3DAUDIO_PI / 4.0f;	// 内部角度の計算に使用される値 ( 0.0f ～ X3DAUDIO_PI/4.0 )

	Emitter.pVolumeCurve		= (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// ボリューム レベル距離カーブ ( NULL でデフォルト値を使う )
	Emitter.pLFECurve			= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;				// LFE ロールオフ距離カーブ ( NULL でデフォルト値を使う )
	Emitter.pLPFDirectCurve		= NULL;														// ローパスフィルター (LPF) ダイレクトパス係数距離カーブ ( NULL でデフォルト値を使う )
	Emitter.pLPFReverbCurve		= NULL;														// LPF リバーブパス係数距離カーブ ( NULL でデフォルト値を使う )
	Emitter.pReverbCurve		= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// リバーブ センド レベル距離カーブ ( NULL でデフォルト値を使う )
	Emitter.CurveDistanceScaler	= 10.0f;													// 正規化された距離カーブをスケーリングするために使用するカーブ距離スケーラ ( FLT_MIN ～ FLT_MAX )
	Emitter.DopplerScaler		= 1.0f;													// ドップラー偏移効果を強調するために使用するドップラー偏移スケーラー ( 0.0f ～ FLT_MAX )
	
	//  X3DAudioCalculate の呼び出し結果を受け取る構造体の初期化
	memset( &DSPSettings, 0, sizeof( DSPSettings ) );
	memset( MatrixCoefficients, 0, sizeof( FLOAT32 ) * input_channels * OUTPUTCHANNELS );
	DSPSettings.SrcChannelCount		= input_channels;
	DSPSettings.DstChannelCount		= 0;//output_channels_;     // calc でセットしておく
	DSPSettings.pMatrixCoefficients	= MatrixCoefficients;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EmitterState::~EmitterState()
{
    LN_SAFE_DELETE_ARRAY( EmitterAzimuths );
	LN_SAFE_DELETE_ARRAY(MatrixCoefficients);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EmitterState::UpdateXAudioEmitter( float scale )
{
    Emitter.Position.x = Position.X * scale;
    Emitter.Position.y = Position.Y * scale;
    Emitter.Position.z = Position.Z * scale;
    Emitter.Velocity.x = Velocity.X * scale;
    Emitter.Velocity.y = Velocity.Y * scale;
    Emitter.Velocity.z = Velocity.Z * scale;
    Emitter.CurveDistanceScaler = Distance * scale;
}

} // namespace Audio
} // namespace Lumino
