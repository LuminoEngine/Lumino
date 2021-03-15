#pragma once

namespace ln {
namespace detail {

class IAudioDeviceRenderCallback
{
public:
	// 現在はステレオのみ。length / 2 でチャンネルのサンプル数がわかる。
	virtual void render(float* outputBuffer, int length) = 0;
};

class AudioDevice
	: public RefObject
{
public:
	AudioDevice();
	virtual ~AudioDevice() = default;
	void init(int frames, int channels);
	virtual void dispose();

	const AudioListenerParams& listener() const { return m_listener; }

	virtual int deviceSamplingRate() = 0;

	void setRenderCallback(IAudioDeviceRenderCallback* callback) { m_renderCallback = callback; }

	// 毎フレーム更新用。スレッド使う場合はそこからよばれる。
	virtual void updateProcess() = 0;

	virtual void run() {}
protected:
	void render(int16_t* outputBuffer, int length);

private:
	IAudioDeviceRenderCallback* m_renderCallback;
	AudioListenerParams m_listener;
	int m_masterChannels;
	std::vector<float> m_renderdBuffer;
    ElapsedTimer m_failoutTimer;
    uint64_t m_failoutMilliseconds;
};

class NullAudioDevice
	: public AudioDevice
{
public:
	NullAudioDevice();
	virtual int deviceSamplingRate() override;
	virtual void updateProcess() override;

private:
};

} // namespace detail
} // namespace ln

