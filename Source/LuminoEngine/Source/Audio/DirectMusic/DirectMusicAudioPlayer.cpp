
#include "../Internal.h"
#include "DirectMusicAudioDevice.h"
#include "DirectMusicAudioPlayer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
namespace detail
{

//==============================================================================
// DirectMusicAudioPlayer
//==============================================================================

//------------------------------------------------------------------------------
DirectMusicAudioPlayer::DirectMusicAudioPlayer( AudioDevice* manager )
	: AudioPlayer(manager)
	, m_midiDecoder(NULL)
	, m_segment(NULL)
	, m_pauseTime(0)
{
    //LN_SAFE_ADDREF( mManager );
}

//------------------------------------------------------------------------------
DirectMusicAudioPlayer::~DirectMusicAudioPlayer()
{
	LN_SAFE_DELETE(m_segment);

	//if (mManager)
	//{
	//	mManager->removeAudioPlayer(this);
	//	LN_SAFE_RELEASE(mManager);
	//}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::initialize(AudioStream* audioStream, bool enable3d)
{
	m_midiDecoder = dynamic_cast<MidiDecoder*>(audioStream->getDecoder());
	if (LN_REQUIRE(m_midiDecoder != NULL)) return;

	AudioPlayer::initialize(audioStream, enable3d);

	// オンメモリ再生用に内部に持ってるバッファを埋める
	m_midiDecoder->fillOnmemoryBuffer();

	if (DirectMusicManager::getInstance()->isInitialized() && !m_segment)
	{
		m_segment = LN_NEW DirectMusicSegment(
			DirectMusicManager::getInstance(),
			DirectMusicManager::getInstance()->createDMPerformance(),
			m_midiDecoder);
	}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::setVolume(float volume)
{
	AudioPlayer::setVolume(volume);
	if (m_segment) {
		m_segment->setVolume(mVolume);
	}
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::setPitch(float pitch)
{
	AudioPlayer::setPitch(pitch);
	if (m_segment) {
		m_segment->setPitch(mPitch);
	}
}

//------------------------------------------------------------------------------
uint64_t DirectMusicAudioPlayer::getPlayedSamples() const
{
	if (m_segment) {
		return m_segment->getPlayPosition();
	}
	return 0;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::setLoopState(uint32_t loopBegin, uint32_t loopLength)
{
	mLoopBegin = loopBegin;
	mLoopLength = loopLength;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::play()
{
	if (!DirectMusicManager::getInstance()->isInitialized())
	{
		// 同じものは追加されないのでこのままで OK
		DirectMusicManager::getInstance()->addPlayRequest(this);
		return;
	}

	// 再生開始
	_play();
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::stop()
{
	if (m_segment) {
		m_segment->stop();
	}
	mIsPlaying = false;
	mIsPausing = false;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::pause(bool isPause)
{
	// 再生中の場合
	if (mIsPlaying)
	{
		// 一時停止する場合で、現在一時停止していない場合
		if (isPause && !mIsPausing)
		{
			// 現在の演奏位置を記憶して停止
			m_pauseTime = m_segment->getPlayPosition();
			m_segment->stop();
			mIsPausing = true;
		}
		else if (!isPause && mIsPausing)
		{
			// 一時停止した時の再生位置に移動
			m_segment->setPlayPosition(m_pauseTime);
			m_segment->play();
			mIsPausing = false;
		}
	}
}

//------------------------------------------------------------------------------
bool DirectMusicAudioPlayer::polling()
{
	// 再生中ではない場合は中断
	if (!mIsPlaying) {
		return false;
	}
	// ポーズ中は何もしないが再生中扱いとする
	if (mIsPausing) {
		return true;
	}

	// ループ再生ではない場合
	if (!mIsLoop)
	{
		if (m_segment)
		{
			// 再生開始直後は IDirectMusicPerformance8::IsPlaying() は false になってしまい、
			// TotalTime() も実際に音が鳴る長さよりも若干短くなってしまっている。
			// (DirectMusic の仕様？というか、バグな気もする…)
			// そのため、音が鳴っている事と、再生位置による二重判定を行う。
			if (!m_segment->isPlaying() && m_segment->getPlayPosition() >= m_segment->getTotalTime())
			{
				stop();
				return false;
			}
		}
	}

	return true;
}

// 臨時用。全体の再生時間の取得
uint32_t DirectMusicAudioPlayer::getTotalTime() const
{
	if (m_segment) // 未初期化対策
	{
		return m_segment->getTotalTime();
	}
	return 0;
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::onFinishDMInit(IDirectMusicPerformance8* dmPerformance)
{
	m_segment = LN_NEW DirectMusicSegment(DirectMusicManager::getInstance(), dmPerformance, m_midiDecoder);

	// 初期化中に設定されたパラメータを再設定する
	setVolume(mVolume);
	setPitch(mPitch);

	_play();
}

//------------------------------------------------------------------------------
void DirectMusicAudioPlayer::_play()
{
	// とりあえず停止
	stop();

	if (!m_segment)
	{
		m_segment = LN_NEW DirectMusicSegment(
			DirectMusicManager::getInstance(),
			DirectMusicManager::getInstance()->createDMPerformance(),
			m_midiDecoder);

		// ちなみに setAudioSource() で作成するのはダメ。
		// DirectMusic の初期化中に setAudioSource() が呼ばれた場合はもちろん m_segment = NULL だけど、
		// そのあと play() が呼ばれたときにも当然 NULL のまま。
		// 初期化中に一度でも play() を呼んで要求リストに追加していれば onFinishDMInit() が呼ばれるけど、
		// そうでない場合はずっと NULL のままになってしまう。
	}

	// ループ再生する場合
	if (mIsLoop)
	{
		// MIDI のループ位置計算は他のとはちょっと違うのでここで、mLoopBegin とかには格納せずここで計算してしまう
		uint32_t cc111time;
		uint32_t base_time;
		m_midiDecoder->setLoopState(&cc111time, &base_time);

		uint32_t begin = cc111time * LN_MUSIC_TIME_BASE / base_time;
		m_segment->setLoopState(true, begin, 0);	// length=0 で終端まで再生する
	}
	// ループ再生しない場合
	else {
		m_segment->setLoopState(false, 0, 0);
	}

	m_segment->play();

	mIsPlaying = true;
	mIsPausing = false;
}

} // namespace detail
LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
