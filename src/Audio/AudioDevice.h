/*
	@file	AudioDevice.h
*/
#pragma once

#include <Lumino/Base/Cache.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/Audio/Common.h>

namespace Lumino
{
namespace Audio
{
class AudioStream;
class AudioPlayer;

/// デバイス管理と、Player の生成・管理を行うクラスのベースクラス
class AudioDevice
	: public RefObject
{
public:
	AudioDevice() {}
	virtual ~AudioDevice() {}
public:

	/// 3Dサウンドリスナーの取得
	SoundListener* getSoundListener() { return &m_soundListener; }

public:

	/// AudioPlayer を作成する (type に LN_SOUNDPLAYTYPE_AUTO は指定できないので注意)
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayType type) = 0;
	/// 更新 (更新スレッドから呼ばれる)
	virtual void Update() = 0;
	//virtual void SetListenerState(const Vector3& position, const Vector3& front) = 0;
	/// 3D 空間の1メートル相当の距離の設定
	virtual void SetMetreUnitDistance(float d) = 0;

protected:
	SoundListener		m_soundListener;
};

} // namespace Audio
} // namespace Lumino
