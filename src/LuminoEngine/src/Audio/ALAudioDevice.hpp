#pragma once
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
	virtual void updateProcess() override;

private:
	ALCdevice* m_alDevice;
	ALCcontext* m_alContext;
	ALuint m_masterSource;
	std::vector<ALuint> m_freeBuffers;

	int m_sampleRate;
};

} // namespace detail
} // namespace ln

