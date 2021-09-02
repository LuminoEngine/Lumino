#pragma once
#include <shared_mutex>

//#ifdef __EMSCRIPTEN__
//#else
#define LN_AUDIO_THREAD_ENABLED
//#endif

namespace ln {
class AudioNode;
class Sound;

enum class PlayingState	// TODO: AudioPlayingState
{
	NoChanged,
	Stop,
	Play,
	Pause,
};

namespace detail {
class internal_shared_mutex;
using AudioRWMutex = internal_shared_mutex;
using ScopedReadLock = std::shared_lock<ln::detail::AudioRWMutex>;
using ScopedWriteLock = std::lock_guard<ln::detail::AudioRWMutex>;
#define LN_AUDIO_READ_LOCK_COMMIT	std::shared_lock<ln::detail::AudioRWMutex> lock(commitMutex());
#define LN_AUDIO_WRITE_LOCK_COMMIT	std::lock_guard<ln::detail::AudioRWMutex> lock(commitMutex());
} // namespace detail

namespace detail {
class AudioManager;
class AudioDecoder;
class ARINode;
class ARISourceNode;
class ARIGainNode;
class SoundCore;

struct AudioListenerParams
{
	Vector3 m_position;
	Vector3 m_forward;
	Vector3 m_up;
	Vector3 m_velocity;
	float m_dopplerFactor;
	float m_speedOfSound;

	AudioListenerParams();
};

struct AudioEmitterParams
{
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_direction;	// zero is legal value.

	AudioEmitterParams();
};

struct AudioConeParams
{
	float m_innerAngle;
	float m_outerAngle;
	float m_outerGain;

	AudioConeParams();
};

class ARISourceNode;
class ARIDestinationNode;
class ARIPannerNode;

} // namespace detail
} // namespace ln

