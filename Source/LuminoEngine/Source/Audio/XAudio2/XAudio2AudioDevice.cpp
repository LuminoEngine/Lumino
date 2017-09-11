
#include "../Internal.h"
#include "XAudio2AudioPlayer.h"
#include "XAudio2AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// XAudio2AudioPlayerBase 
//==============================================================================

// 背面の音源が減衰されるコーンの設定
static const X3DAUDIO_CONE sListener_DirectionalCone = {
    X3DAUDIO_PI*5.0f/6.0f,
    X3DAUDIO_PI*11.0f/6.0f,
    1.0f, 1.75f,
    0.0f, 0.25f,
    0.708f, 1.0f };

// 音が完全に聞こえなくなる距離 (XACT 使えばこんなのいらないっぽいけど、なんか面倒なので)
static const float OuterDistance    = 14.0f;

//------------------------------------------------------------------------------
XAudio2AudioDevice::XAudio2AudioDevice()
	: m_XAudio(nullptr)
	, m_masteringVoice(nullptr)
	, m_deviceChannels(0)
	, m_metreUnitDistanceInv(1.0f)
{
	memset(m_X3DInstance, 0, sizeof(m_X3DInstance));
}

//------------------------------------------------------------------------------
XAudio2AudioDevice::~XAudio2AudioDevice()
{
    if ( m_masteringVoice )
	{
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = NULL;
	}

    if ( m_XAudio )
    {
		m_XAudio->StopEngine();
		LN_COM_SAFE_RELEASE(m_XAudio);
    }
}

//------------------------------------------------------------------------------
bool XAudio2AudioDevice::initialize(/* const ConfigData& configData */)
{
	if (!m_module.initialize()) {
		return false;
	}

    UINT32 flags = 0;

  //  if ( configData.EnableDebugFlag )
  //  {
		//flags |= XAUDIO2_DEBUG_ENGINE;
  //  }

    // XAudio2 作成
    HRESULT hr = m_module.XAudio2Create(&m_XAudio, flags);
	if (FAILED(hr)) {
		return false;
	}

	// マスターボイス作成
	hr = m_XAudio->CreateMasteringVoice(&m_masteringVoice);
	if (FAILED(hr)) {
		return false;
	}

	ln::Logger::writeLine("get XAudio2AudioDevice information...");

	// デバイス詳細情報の確認
	XAUDIO2_DEVICE_DETAILS details;
	hr = m_XAudio->GetDeviceDetails(0, &details);
	if (SUCCEEDED(hr))
	{
		ln::Logger::writeLine(L"    DeviceID    : %s", details.DeviceID);
		ln::Logger::writeLine(L"    DisplayName : %s", details.DisplayName);
		ln::Logger::writeLine(L"    Role        : %d", details.Role);
		ln::Logger::writeLine(L"    OutputFormat ( WAVEFORMATEX )");
		ln::Logger::writeLine("        Format ID       : %hu", details.OutputFormat.Format.wFormatTag);
		ln::Logger::writeLine("        Channels        : %hu", details.OutputFormat.Format.nChannels);
		ln::Logger::writeLine("        SamplesPerSec   : %lu", details.OutputFormat.Format.nSamplesPerSec);
		ln::Logger::writeLine("        AvgBytesPerSec  : %lu", details.OutputFormat.Format.nAvgBytesPerSec);
		ln::Logger::writeLine("        BlockAlign      : %hu", details.OutputFormat.Format.nBlockAlign);
		ln::Logger::writeLine("        BitsPerSample   : %hu", details.OutputFormat.Format.wBitsPerSample);
		ln::Logger::writeLine("        ExtraSize       : %hu", details.OutputFormat.Format.cbSize);
    }
    else
    {
		ln::Logger::writeLine("failed to get information.");
    }

    // チャンネル数記憶
    m_deviceChannels = details.OutputFormat.Format.nChannels;
		
	m_module.X3DAudioInitialize(
		details.OutputFormat.dwChannelMask,		// Xbox 360 では SPEAKER_XBOX
		X3DAUDIO_SPEED_OF_SOUND, m_X3DInstance );
		//SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER |
        //SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT

    // リスナーの設定
    m_listenerState.OrientFront	= D3DXVECTOR3( 0, 0, 1 );	// 前方向の向き
	m_listenerState.OrientTop	= D3DXVECTOR3( 0, 1, 0 );	// 上方向の向き ( OrientFront と正規直交であること )
	m_listenerState.Position	= D3DXVECTOR3( 0, 0, 0 );
	m_listenerState.Velocity	= D3DXVECTOR3( 0, 0, 0 );
	m_listenerState.pCone		= NULL;//(X3DAUDIO_CONE*)&sListener_DirectionalCone;

    return true;
}

//------------------------------------------------------------------------------
void XAudio2AudioDevice::calcEmitterState(EmitterState* emitter)
{
    if ( emitter )
    {
        static const DWORD calc_flags =
			X3DAUDIO_CALCULATE_MATRIX       |
            X3DAUDIO_CALCULATE_LPF_DIRECT   |
            X3DAUDIO_CALCULATE_LPF_REVERB   |
            X3DAUDIO_CALCULATE_REVERB       |
            X3DAUDIO_CALCULATE_DOPPLER;

		emitter->updateXAudioEmitter(m_metreUnitDistanceInv);
		emitter->DSPSettings.DstChannelCount = m_deviceChannels;

		m_module.X3DAudioCalculate(
			m_X3DInstance,
			&m_listenerState,
			&emitter->Emitter,
			calc_flags,
			&emitter->DSPSettings);
    }
}

//------------------------------------------------------------------------------
AudioPlayer* XAudio2AudioDevice::createAudioPlayer(AudioStream* audioStream, bool enable3d, SoundPlayingMode mode)
{
	Ref<AudioPlayer> audioPlayer;

    // 種類に応じてプレイヤーを作成する
	switch ( mode )
	{
		// オンメモリ再生
		case SoundPlayingMode::OnMemory:
        {
			XAudio2OnMemoryAudioPlayer* player = LN_NEW XAudio2OnMemoryAudioPlayer(this);
			audioPlayer.attach(player, false);
			player->initialize(audioStream, enable3d);
			break;
        }
		// ストリーミング再生
		case SoundPlayingMode::Streaming:
        {
			XAudio2StreamingAudioPlayer* player = LN_NEW XAudio2StreamingAudioPlayer(this);
			audioPlayer.attach(player, false);
			player->initialize(audioStream, enable3d);
			break;
        }
		default:
			LN_UNREACHABLE();
			break;
	}

	audioPlayer.safeAddRef();
	return audioPlayer;
}

//------------------------------------------------------------------------------
void XAudio2AudioDevice::update()
{
	auto* data = getSoundListenerData();
    m_listenerState.OrientFront.x = data->direction.x;
	m_listenerState.OrientFront.y = data->direction.y;
	m_listenerState.OrientFront.z = data->direction.z;
	m_listenerState.OrientTop.x = data->upDirection.x;
	m_listenerState.OrientTop.y = data->upDirection.y;
	m_listenerState.OrientTop.z = data->upDirection.z;
	m_listenerState.Position.x = data->position.x * m_metreUnitDistanceInv;
	m_listenerState.Position.y = data->position.y * m_metreUnitDistanceInv;
	m_listenerState.Position.z = data->position.z * m_metreUnitDistanceInv;
	m_listenerState.Velocity.x = data->velocity.x * m_metreUnitDistanceInv;
	m_listenerState.Velocity.y = data->velocity.y * m_metreUnitDistanceInv;
	m_listenerState.Velocity.z = data->velocity.z * m_metreUnitDistanceInv;
}

//==============================================================================
// EmitterState 
//==============================================================================

static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[ 3 ]		= { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[ 3 ]	= { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
EmitterState::~EmitterState()
{
	LN_SAFE_DELETE_ARRAY(EmitterAzimuths);
	LN_SAFE_DELETE_ARRAY(MatrixCoefficients);
}
//------------------------------------------------------------------------------
void EmitterState::updateXAudioEmitter( float scale )
{
    Emitter.Position.x = Position.x * scale;
    Emitter.Position.y = Position.y * scale;
    Emitter.Position.z = Position.z * scale;
    Emitter.Velocity.x = Velocity.x * scale;
    Emitter.Velocity.y = Velocity.y * scale;
    Emitter.Velocity.z = Velocity.z * scale;
    Emitter.CurveDistanceScaler = distance * scale;
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
