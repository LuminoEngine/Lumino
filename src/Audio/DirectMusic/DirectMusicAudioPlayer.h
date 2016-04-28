
#pragma once
#include "DirectMusic.h"
#include "../AudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class AudioDevice;

/*
	@brief	Midi データの再生を行うクラス
	@note	DirectMusic を別スレッドで初期化する場合、フェードイン・アウト時間の
			つじつまが合わなくなるということがあります。
			これは「DirectMusic 初期化中は音を鳴らすことができないため、
			「再生中ではない」という判定が入り、停止中はフェード処理がされないために起こります。
			これを修正し、Midi の演奏が開始される時間に合わせて正しい音量を
			設定するという事は可能でしたが、実際にこの不具合に遭遇するような事態というのは
			ユーザーの方が「ゲーム開始時からフェードイン再生したい」というものがほとんどです。
			これを修正した場合、開始時に中途半端な音量から演奏を開始することになり、
			演出としてあまり良くないと考えたため、現段階ではこれは仕様とします。
*/
class DirectMusicAudioPlayer
    : public AudioPlayer
    , public DirectMusicManager::PlayerObject
{
public:
	DirectMusicAudioPlayer(AudioDevice* device);
	virtual ~DirectMusicAudioPlayer();

	// AudioPlayer interface
	virtual void			Initialize(AudioStream* audioStream, bool enable3d) override;
	virtual void			SetVolume(float volume) override;
	virtual void			SetPitch(float pitch) override;
	virtual void			SetLoopState(uint32_t loop_begin, uint32_t loop_length) override;
	virtual uint64_t		GetPlayedSamples() const override;
	virtual void			Play() override;
	virtual void			Stop() override;
	virtual void			Pause(bool is_pause) override;
	virtual bool			Polling() override;
	virtual bool			Is3DSound()  override { return false; }
	virtual void			setPosition(const Vector3& pos) override { }
	virtual const Vector3&	getPosition() override { return Vector3::Zero; }
	virtual void			setVelocity(const Vector3& v) override { }
	virtual void			setEmitterDistance(float distance) override { }
	virtual float			getEmitterDistance() const override { return 0; }

    uint32_t getTotalTime() const;

	// DirectMusicManager::PlayerObject interface
	virtual void onFinishDMInit(IDirectMusicPerformance8* dm_performance) override;

private:

	/// 実際に再生する
    void _play();

	//AudioDevice*		m_device;		///< 管理クラス
	MidiDecoder*		m_midiDecoder;	///< MidiDecoder* にキャストした AudioDecoder
    DirectMusicSegment*	m_segment;		///< DirectMusic の再生管理
    uint32_t			m_pauseTime;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
