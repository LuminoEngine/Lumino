
#pragma once
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{
class AudioStream;
class AudioPlayer;

// 3D サウンドリスナー
struct SoundListenerData
{
	Vector3		position;
	Vector3		direction;
	Vector3		upDirection;
	Vector3		velocity;

	SoundListenerData()
		: position(0.0f, 0.0f, 0.0f)
		, direction(0.0f, 0.0f, 1.0f)
		, upDirection(0.0f, 1.0f, 0.0f)
		, velocity(0.0f, 0.0f, 0.0f)
	{}
};

// デバイス管理と、Player の生成・管理を行うクラスのベースクラス
class AudioDevice
	: public RefObject
{
public:
	AudioDevice() {}
	virtual ~AudioDevice() {}

	// 3Dサウンドリスナーの取得
	SoundListenerData* GetSoundListenerData() { return &m_soundListenerData; }

	// AudioPlayer を作成する (type に LN_SOUNDPLAYTYPE_AUTO は指定できないので注意)
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) = 0;
	
	// 更新 (更新スレッドから呼ばれる)
	virtual void Update() = 0;
	
	// 3D 空間の1メートル相当の距離の設定
	virtual void SetMetreUnitDistance(float d) = 0;

private:
	SoundListenerData		m_soundListenerData;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
