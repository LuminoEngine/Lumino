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
	virtual void dispose();

	void setRenderCallback(IAudioDeviceRenderCallback* callback) { m_renderCallback = callback; }

	// 毎フレーム更新用。スレッド使う場合はそこからよばれる。
	virtual void updateProcess() = 0;

protected:
	void render(float* outputBuffer, int length);

private:
	IAudioDeviceRenderCallback* m_renderCallback;
};

} // namespace detail
} // namespace ln

