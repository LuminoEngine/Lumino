/*
	@file	AudioStream.h
*/
#pragma once

#include <Lumino/Base/Exception.h>
#include <Lumino/Base/Cache.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Audio/Common.h>

namespace Lumino
{
namespace Audio
{
class AudioDecoder;

class AudioStream
	: public ASyncIOObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	AudioStream(Stream* stream);
	virtual ~AudioStream();

	void Create() { InvokeIOProc(true, &FileManager::GetInstance()); }

	/// 初期化が完了しているか確認する (例外が発生していればここから再 throw される)
	bool CheckCreated();

	/// デコーダの取得
	AudioDecoder* GetDecoder() { return m_decoder; }
	const AudioDecoder* GetDecoder() const { return m_decoder; }

protected:
	///　非同期ロード処理
	virtual void OnASyncIOProc();

private:
	Stream*			m_stream;
	AudioDecoder*	m_decoder;
};

/// 音声データのベースクラス
class AudioDecoder
	: public RefObject
{
protected:
	AudioDecoder();
	virtual ~AudioDecoder();

//public:
//	/// Create 済みかを確認する。例外が保存されていれば throw する (非同期読み込み用)
//	bool CheckCreated();
//
//protected:
//	friend class ASyncAudioStreamLoadTask;
//	void OnCreateFinished(Exception* e);

public:
	/// 作成
	virtual void Create(Stream* stream) = 0;

	/// ファイルフォーマットの取得
	virtual StreamFormat GetSourceFormat() const = 0;

	/// PCM フォーマットの取得
	virtual const WaveFormat* GetWaveFormat() const = 0;

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual uint32_t GetSourceDataSize() const = 0;

	/// 全体の再生時間の取得 ( ミリ秒。後で無くなるかも )
	//virtual uint32_t getTotalTime() const = 0;

	/// 全体のサンプル数の取得 ( Midi の場合はミュージックタイム単位 )
	virtual uint32_t GetTotalUnits() const = 0;

	/// オンメモリ再生用のバッファの先頭アドレス取得 ( fillBufferAndReleaseStream() を呼んでいない場合は NULL )
	virtual byte_t* GetOnmemoryPCMBuffer() const = 0;

	/// オンメモリ再生時の全バッファサイズの取得
	virtual uint32_t GetOnmemoryPCMBufferSize() const = 0;

	/// 1 秒分のソースデータをデコードするときの、最適なバイト数の取得
	///	
	///	通常は PCM フォーマットから取得できるけど、MP3 の場合は
	///	API の都合(?)上、デコードに最適な 1 秒分のサイズは、普通のPCMのそれとは異なる。
	///	そのため、ちゃんとチェックできるようにこのメソッドを用意。
	///	いまのところは MP3 に限った話だけど、GetWaveFormat() で
	///	取得した値から 1 秒分のサイズを計算するとバグので注意。
	virtual uint32_t GetBytesPerSec() const = 0;

	/// ループ開始位置と終了位置の取得
	///
	///	Midi ファイルの場合は最初の CC111 位置のデルタタイムとベースタイム
	virtual void GetLoopState(uint32_t* begin, uint32_t* length) const = 0;

	/// オンメモリ再生用に全てのデータを読み込む
	///
	/// デコード後の PCM データサイズ分のバッファを内部で確保し、
	/// そこに全てのデータを読み込みます。
	/// データ及びサイズは getOnmemoryPCMBuffer()、
	/// getOnmemoryPCMBufferSize() で取得してください。<br>
	/// <br>
	/// 複数回呼ばれても、すでにバッファが確保されている場合は
	/// なにもしません。<br>
	/// <br>
	/// 呼出し後、ストリームは解放され、このオーディオソースは
	/// ストリーミング再生には使用できなくなります。<br>
	//virtual void fillBufferAndReleaseStream() = 0;
	/// ※スレッドセーフで実装する
	virtual void FillOnmemoryBuffer() = 0;

	/*
		データをデコードし、buffer に書き込む
		@param[in]	seekPos			: シーク位置
		@param[out]	buffer			: PCM データを書き込むバッファ
		@param[in]	bufferSize		: buffer のサイズ (バイト単位)
		@param[in]	outReadSize		: ソースストリームから読み込んだデータサイズ (現在のシーク位置にこの値を加算した値が、次の読み取りシーク位置になる)
		@param[in]	outWriteSize	: 実際に buffer に書き込んだサイズ (デコードされたPCMデータのサイズ = デバイスに渡すバイト数)
		@details	できるだけ buffer_size を満たすようにデータをデコードし、buffer 書き込みます。
					outReadSize はデコードの為にソースストリームから読み込んだデータサイズです。
					通常、mp3 等の圧縮フォーマットでは outWriteSize よりも小さい値になります。
					現在のファイルポインタに outReadSize の値を足した値が、次回の読み込み位置となります。
					この関数はスレッドセーフです。
	*/
	virtual void Read(uint32_t seekPos, void* buffer, uint32_t bufferSize, uint32_t* outReadSize, uint32_t* outWriteSize) = 0;

	/// ファイルポインタ移動 (先頭からのバイトオフセット)
	/// (このクラスで実装している read() と seek() は getOnmemoryPCMBuffer() に読み込んでいる事が前提)
	//virtual void seek(uint32_t offset);

	/// デコード状態のリセット(再生開始直前に呼ばれる。MP3 用)
	virtual void Reset() = 0;

private:
	Exception*	m_exception;
	bool		m_finishedCreate;
};

} // namespace Audio
} // namespace Lumino
