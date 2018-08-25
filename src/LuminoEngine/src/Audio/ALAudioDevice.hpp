#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include "AudioDevice.hpp"

namespace ln {
namespace detail {

class ALAudioDevice
	: public AudioDevice
{
public:
	ALAudioDevice();
	virtual ~ALAudioDevice() = default;
	void initialize();
	virtual void dispose() override;
	virtual int deviceSamplingRate() override;
	virtual void updateProcess() override;

private:
	ALCdevice* m_alDevice;
	ALCcontext* m_alContext;
	ALuint m_masterSource;
	std::vector<ALuint> m_freeBuffers;
	//std::vector<float> m_renderdBuffer;
	//std::vector<int16_t> m_finalRenderdBuffer;

	int m_masterSampleRate;
	std::vector<int16_t> m_finalRenderdBuffer;
};

} // namespace detail
} // namespace ln

