
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

namespace detail
{
	
/*
	別スレッドでのデコード・再生管理を行いたいので、別スレッドで処理中に各種ステートを変更してほしくない。
	Polling でステートを丸ごとコピーしておく。
	Sound クラスの持つステートを変更する関数は全て mutex 張らなければならない。読み取りは無くてもいい。
*/
class AudioPlayerState
{
public:
	enum ModifiedFlags
	{
		ModifiedFlags_None = 0x0000,
		ModifiedFlags_Volume = 0x0001,
		ModifiedFlags_Pitch = 0x0002,
		ModifiedFlags_LoopEnabled = 0x0004,
		ModifiedFlags_LoopBegin = 0x0008,
		ModifiedFlags_LoopLength = 0x0010,
		ModifiedFlags_PlayingState = 0x0020,
	};

public:
	AudioPlayerState();

	void				SetModifiedFlags(uint32_t v) { m_modified = v; }
	uint32_t			GetModifiedFlags() const { return m_modified; }
	void				SetVolume(float v) { m_volume = v; m_modified |= ModifiedFlags_Volume; }
	float				GetVolume() const { return m_volume; }
	void				SetPitch(float v) { m_pitch = v; m_modified |= ModifiedFlags_Pitch; }
	float				GetPitch() const { return m_pitch; }
	void				SetLoopEnabled(bool v) { m_loopEnabled = v; m_modified |= ModifiedFlags_LoopEnabled; }
	bool				IsLoopEnabled() const { return m_loopEnabled; }
	void				SetLoopBegin(uint32_t v) { m_loopBegin = v; m_modified |= ModifiedFlags_LoopBegin; }
	uint32_t			GetLoopBegin() const { return m_loopBegin; }
	void				SetLoopLength(uint32_t v) { m_loopLength = v; m_modified |= ModifiedFlags_LoopLength; }
	uint32_t			GetLoopLength() const { return m_loopLength; }
	void				SetPlayingState(SoundPlayingState v) { m_playingState = v; m_modified |= ModifiedFlags_PlayingState; }
	SoundPlayingState	GetPlayingState() const { return m_playingState; }

private:
	uint32_t			m_modified;			// ModifiedFlags
	float				m_volume;			// 音量
	float				m_pitch;			// ピッチ
	bool			    m_loopEnabled;		// ループ有無
	uint32_t			m_loopBegin;		// ループされる領域の最初のサンプル (Midi なら ミュージックタイム単位)
	uint32_t			m_loopLength;		// ループ領域の長さ (サンプル数単位)  (Midi なら ミュージックタイム単位)
	SoundPlayingState	m_playingState;		// 再生状態
};

} // namespace detail

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
