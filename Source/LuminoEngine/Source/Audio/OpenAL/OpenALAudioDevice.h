
#pragma once
#include "../AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// OpenAL 用の AudioDevice の実装
class OpenALAudioDevice
    : public AudioDevice
{
public:
	OpenALAudioDevice();
	virtual ~OpenALAudioDevice();
    bool Initialize();
    void Finalize();

public:
	// AudioDevice interface
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) override;
    virtual void Update() override;
    virtual void SetMetreUnitDistance( float d ) override { m_metreUnitDistanceInv = 1.0f / d; }

private:
	float	m_metreUnitDistanceInv;		// 3D 空間の1メートル相当の距離の逆数
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
