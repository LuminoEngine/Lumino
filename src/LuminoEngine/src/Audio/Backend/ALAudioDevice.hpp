#pragma once

#ifdef LN_USE_OPENAL

#if defined(__APPLE__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "AudioDevice.hpp"

namespace ln {
namespace detail {

class ALAudioDevice
	: public AudioDevice
{
public:
	ALAudioDevice();
	virtual ~ALAudioDevice() = default;
	void init(int frameLength);
	virtual void dispose() override;
	virtual int deviceSamplingRate() override;
	virtual void updateProcess() override;

private:
	ALCdevice* m_alDevice;
	ALCcontext* m_alContext;
	ALuint m_masterSource;
	std::vector<ALuint> m_freeBuffers;
	std::vector<int16_t> m_finalRenderdBuffer;
};

} // namespace detail
} // namespace ln

#endif
