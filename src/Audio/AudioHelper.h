
#pragma once

namespace Lumino
{
namespace Audio
{
class Sound;

class AudioHelper
{
public:
	static inline void SetGameAudioFlags(Sound* sound, uint32_t flags) { sound->m_gameAudioFlags = flags; }
	static inline uint32_t GetGameAudioFlags(Sound* sound) { return sound->m_gameAudioFlags; }
	static inline AudioStream* GetAudioStream(Sound* sound) { return sound->m_audioStream; }
	static inline Sound* CreateSound(AudioManager* manager, AudioStream* stream) { return LN_NEW Sound(manager, stream); }
};

} // namespace Audio
} // namespace Lumino
