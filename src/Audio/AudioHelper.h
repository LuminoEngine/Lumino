
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class Sound;

class AudioHelper
{
public:
	static inline void SetGameAudioFlags(Sound* sound, uint32_t flags) { sound->m_gameAudioFlags = flags; }
	static inline uint32_t GetGameAudioFlags(Sound* sound) { return sound->m_gameAudioFlags; }
	static inline AudioStream* GetAudioStream(Sound* sound) { return sound->m_audioStream; }
	//static inline Sound* CreateSound(AudioManagerImpl* manager, AudioStream* stream) { return LN_NEW Sound(manager, stream); }
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
