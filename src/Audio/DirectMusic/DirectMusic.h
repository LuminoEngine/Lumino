
#pragma once

#include <list>
#include <Lumino/Threading/Thread.h>
#include "../MidiDecoder.h"
#include "lndmusic.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

class DirectMusicManager;

/**
	@brief      DirectMusic 上での再生シーケンスひとつ分を扱うクラス

	@details	音声ひとつ分を再生するために必要な一連のインターフェースを扱うクラスです。
				(Performance、Segment 等をまとめたもの)

	@note		DirectMusic はパフォーマンスを作成する前にセグメントを作成することができる。
				スレッドで DirectMusic を初期化する場合、初期化前にセグメントを作成しておけば時間の節約とかになるかもしれないけど、
				セグメントをパフォーマンスよりも先に作成すると不具合が起こる。(全部ピアノ音源になるとか)
*/
class DirectMusicSegment
	: public RefObject
{
public:
	/// (midiStream は保持しないため、参照カウントは変化させません。ただし、コンストラクタに渡す前に fillBuffer() を呼んでおく必要があります)
	DirectMusicSegment(DirectMusicManager* manager, IDirectMusicPerformance8* dmPerformance, MidiDecoder* midiStream);
	virtual ~DirectMusicSegment();

public:

	/// 音量の設定 (0.0～1.0)
	void SetVolume(float volume);

	/// ピッチの設定 (0.5 ～ 2.0 が基本。範囲外も一応可。)
	void SetPitch(float pitch);

	/// 全体の再生時間の取得 (ミュージックタイム単位)
    uint32_t GetTotalTime() const;

	/// 再生位置の取得 (ミュージックタイム単位)
	uint32_t GetPlayPosition() const;

	/// 再生位置の設定 (ミュージックタイム単位)
	void SetPlayPosition(uint32_t time);

	/// 再生中かを判定 (音が鳴っているか)
    bool IsPlaying() const;

	/// ループ情報の設定
	///		begin と length はミュージックタイム単位。
	///		 length が 0 の場合は begin の位置から終端まで再生する。
	void SetLoopState( bool isLoop, uint32_t begin, uint32_t length );

	/// 再生
    void Play();

	/// 停止
    void Stop();

private:
    IDirectMusicPerformance8*   m_dmPerformance;
    IDirectMusicAudioPath*      m_dmAudioPath;
    IDirectMusicSegment8*		m_dmSegment;
	IDirectMusicSegmentState8*	m_dmSegmentState;
    IDirectSoundBuffer8*        m_dsSoundBuffer;
};


/**
	@brief      DirectMusic 上での再生シーケンスひとつ分を扱うクラス
	@details	IDirectMusic8 インターフェイスはひとつのアプリケーションにつき
				ひとつしか作成できないため、このクラスはシングルトンで実装。
				http://msdn.microsoft.com/ja-jp/library/cc353890.aspx
*/
class DirectMusicManager
    : private NonCopyable
{
public:
	static const int MusicTimeBase = 768;

public:

	/// 渡す初期化データ
    struct ConfigData
    {
        HWND				WindowHandle;
		DirectMusicMode		DMInitMode;
		float				ReverbLevel;
    };

	/// 再生要求を持つクラスの基底
    class PlayerObject
    {
    public:
        PlayerObject(){}
        virtual ~PlayerObject();

        /// 再生要求リストに追加されている状態で DirectMusic の初期化が完了したときに呼ばれる
        virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance_ ) = 0;
    };

public:

	/// 初期化
	///		DMInitMode が LN_DMINITMODE_NOTUSE の場合はインスタンスを作成しない。GetInstance() は NULL を返す。
	static void Initialize( const ConfigData& configData );

	/// 終了処理
	static void Finalize();

	///インスタンスへのポインタの取得
    static DirectMusicManager* GetInstance() { return m_instance; }

public:

	/// 初期化が完了しているかを判定する
	bool IsInitialized() const { return m_performanceInited.IsTrue(); }

	/// エラー状態の取得 (エラー番号(内部用)。 正常な場合は 0)
    uint32_t GetErrorState();

	float GetReverbLevel() const { return m_reverbLevel; }

	/// 再生要求の追加 (既に同じオブジェクトが追加されている場合はなにもしません。)
	void AddPlayRequest( PlayerObject* obj );

	///  再生要求の削除
	///		DirectMusic の初期化中にオーディオプレイヤーが解放された
	///		時に、この関数でリストから外しますが、
	///		PlayerObject クラスのデストラクタで呼ばれるため
	///		通常は呼び出す必要はありません。
	void RemovePlayRequest( PlayerObject* obj );

	/// IDirectMusicPerformance8 の作成
	///		isInitialized() によって初期化の完了を確認してから呼び出してください。
    IDirectMusicPerformance8* CreateDMPerformance();

	/// 再生要求リストをチェックして、可能なら再生する
    void Polling();

private:
    DirectMusicManager();
    ~DirectMusicManager();

	/// 初期化
    void InternalInitialize( const ConfigData& configData );

	/// ひとつめのパフォーマンスの初期化 ( 別スレッドで初期化する場合はそこから呼ばれる )
	void InitPerformance();

	/// (参照カウントなどに変化はなし)
	//IDirectMusicPerformance8* InternalCreateDMPerformance();

	/// 最初の IDirectMusicPerformance8 の初期化スレッド
	void Thread_InitPerformance();

private:

	//
 //   class PerformanceInitThread
 //       : public Threading::Thread
 //   {
	//public:
 //       PerformanceInitThread(){}

 //       virtual void run()
 //       {
 //           CoInitializeEx( NULL, COINIT_MULTITHREADED );
	//		DirectMusicManager::getInstance()->_initPerformance();
	//		CoUninitialize();
 //       }
 //   };

private:

	typedef Array<PlayerObject*> PlayRequestList;

    HWND						m_windowHandle;
	DirectMusicMode				m_initMode;
	IDirectSound8*				m_directSound;
	IDirectMusic8*				m_directMusic;
	IDirectMusicPerformance8*	m_firstPerformance;	///< 初期化スレッドによって作成された IDirectMusicPerformance8

	Threading::DelegateThread	m_initThread;		///< 初期化スレッド
    Threading::EventFlag		m_performanceInited;     ///< 初期化済みフラグ
    Threading::Mutex			m_mutex;
    Threading::Mutex			m_listLock;

    PlayRequestList				m_playRequestList;
    uint32_t					mErrorState;
	float						m_reverbLevel;

	static DirectMusicManager*	m_instance;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
