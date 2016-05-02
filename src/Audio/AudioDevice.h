
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{
class AudioStream;
class AudioPlayer;

/// 3D サウンドリスナー
struct SoundListenerData
{
	Vector3		Position;
	Vector3		Direction;
	Vector3		UpDirection;
	Vector3		Velocity;

	SoundListenerData()
		: Position(0, 0, 0)
		, Direction(0, 0, 1.0f)
		, UpDirection(0, 1.0f, 0)
		, Velocity(0, 0, 0)
	{}
};

/// デバイス管理と、Player の生成・管理を行うクラスのベースクラス
class AudioDevice
	: public RefObject
{
public:
	AudioDevice() {}
	virtual ~AudioDevice() {}
public:

	/// 3Dサウンドリスナーの取得
	SoundListenerData* getSoundListenerData() { return &m_soundListenerData; }

public:

	/// AudioPlayer を作成する (type に LN_SOUNDPLAYTYPE_AUTO は指定できないので注意)
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) = 0;
	/// 更新 (更新スレッドから呼ばれる)
	virtual void Update() = 0;
	//virtual void SetListenerState(const Vector3& position, const Vector3& front) = 0;
	/// 3D 空間の1メートル相当の距離の設定
	virtual void SetMetreUnitDistance(float d) = 0;

protected:
	SoundListenerData		m_soundListenerData;
};

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
