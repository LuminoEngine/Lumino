
#pragma once
#include "XAudio2Module.h"
#include "../AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// 3D オーディオの計算に必要なパラメータ
struct EmitterState
{
public:
	static const int OUTPUTCHANNELS = 8;
	
public:
    Vector3		Position;
    Vector3		Velocity;
    float		Distance; 

public:
	EmitterState(uint32_t input_channels_);
	~EmitterState();

	/// Position から X3DAUDIO_EMITTER に正しい座標を設定する
	void UpdateXAudioEmitter(float scale);

public:
    X3DAUDIO_EMITTER		Emitter;
    X3DAUDIO_CONE			EmitterCone;
    FLOAT32*				EmitterAzimuths;
    X3DAUDIO_DSP_SETTINGS	DSPSettings;            // この中の DstChannelCount は AudioDevice::update3DState でセットされる
    FLOAT32*				MatrixCoefficients;
};

// XAudio2 用の AudioDevice の実装
class XAudio2AudioDevice
    : public AudioDevice
{
public:
	XAudio2AudioDevice();
	virtual ~XAudio2AudioDevice();
    bool Initialize();
    void Finalize();

	IXAudio2* GetXAudio2() const { return m_XAudio; }
	IXAudio2MasteringVoice* GetMasteringVoice() { return m_masteringVoice; }
	void CalcEmitterState(EmitterState* emitter);

public:
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) override;
    virtual void Update() override;
    virtual void SetMetreUnitDistance( float d ) override { m_metreUnitDistanceInv = 1.0f / d; }

private:
	XAudio2Module				m_module;
    IXAudio2*				    m_XAudio;				// IXAudio2 インスタンスへのポインタ
	IXAudio2MasteringVoice*	    m_masteringVoice;		// IXAudio2MasteringVoice インスタンスへのポインタ
	X3DAUDIO_HANDLE			    m_X3DInstance;			// 3D オーディオの管理クラス
    uint32_t					m_deviceChannels;		// アクティブなオーディオデバイスの出力チャンネル数
    
    X3DAUDIO_LISTENER			m_listenerState;
    float						m_metreUnitDistanceInv;	// 3D 空間の1メートル相当の距離の逆数
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
