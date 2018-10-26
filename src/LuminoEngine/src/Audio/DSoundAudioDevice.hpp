#pragma once

#ifdef LN_OS_WIN32

#include <dsound.h>
#include "AudioDevice.hpp"

namespace ln {
namespace detail {

class DSoundAudioDevice
	: public AudioDevice
{
public:
	DSoundAudioDevice();
	virtual ~DSoundAudioDevice() = default;
	void initialize(int frameLength);
	virtual void dispose() override;
	virtual int deviceSamplingRate() override;
	virtual void updateProcess() override;

private:
	IDirectSound8 * m_dsound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer* m_secondaryBuffer;
	size_t m_nextSectorNumber;
	size_t m_sctorSize;
};

} // namespace detail
} // namespace ln

#endif
