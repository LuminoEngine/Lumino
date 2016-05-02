
#pragma once
#include "AudioPlayer.h"
#include "AudioDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

// 実際に再生を行わないダミーインターフェイス
class NullAudioPlayer
    : public AudioPlayer
{
public:
	NullAudioPlayer(AudioDevice* device);
	virtual ~NullAudioPlayer();

	// AudioPlayer interface
	virtual uint64_t GetPlayedSamples() const override { return 0; }
	virtual void Play() override {}
	virtual void Stop() override {}
	virtual void Pause(bool isPause) override {}
	virtual bool Is3DSound() override { return false; }
	virtual void setPosition(const Vector3& pos) override {}
	virtual const Vector3& getPosition() override { return Vector3::Zero; }
	virtual void setVelocity(const Vector3& v) override {}
	virtual void setEmitterDistance(float distance) override {}
	virtual float getEmitterDistance() const override { return 0; }
	virtual bool Polling() override { return false; }
};

// 実際に再生を行わないダミーインターフェイス
class NullAudioDevice
    : public AudioDevice
{
public:
	NullAudioDevice();
	virtual ~NullAudioDevice();

	// AudioDevice interface
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) override;
	virtual void Update() override;
	virtual void SetMetreUnitDistance(float d) override;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END

