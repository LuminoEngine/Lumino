
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
	virtual uint64_t getPlayedSamples() const override { return 0; }
	virtual void play() override {}
	virtual void stop() override {}
	virtual void pause(bool isPause) override {}
	virtual bool is3DSound() override { return false; }
	virtual void setPosition(const Vector3& pos) override {}
	virtual const Vector3& getPosition() override { return Vector3::Zero; }
	virtual void setVelocity(const Vector3& v) override {}
	virtual void setEmitterDistance(float distance) override {}
	virtual float getEmitterDistance() const override { return 0; }
	virtual bool polling() override { return false; }
};

// 実際に再生を行わないダミーインターフェイス
class NullAudioDevice
    : public AudioDevice
{
public:
	NullAudioDevice();
	virtual ~NullAudioDevice();

	// AudioDevice interface
	virtual AudioPlayer* createAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) override;
	virtual void update() override;
	virtual void setMetreUnitDistance(float d) override;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END

