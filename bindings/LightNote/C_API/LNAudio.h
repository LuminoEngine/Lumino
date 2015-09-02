
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

extern "C" {

/**
	@brief	音声機能
*/
LN_MODULE(Audio)
	
//==============================================================================
/**
	@brief	音声再生のユーティリティクラスです。
*/
LN_STATIC_CLASS(LNAudio)
	
	/**
		@brief		BGM を演奏する
		@param[in]	filePath	: ファイル名
		@param[in]	volume		: ボリューム (0 ～ 100)
		@param[in]	pitch		: ピッチ (50 ～ 200)
		@param[in]	fadeTime	: フェードインにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGM( const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), int fadeTime LN_DEFAULT_ARG(0) );
	
	/**
		@brief	  メモリ上の音声データから BGM を演奏する
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGMMem( const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), int fadeTime LN_DEFAULT_ARG(0) );

	/**
		@brief	  BGM の演奏を停止する
		@param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_StopBGM(int fadeTime LN_DEFAULT_ARG(0));

	/**
		@brief	  BGS を演奏する
		@param[in]  filePath	: ファイル名
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGS( const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), int fadeTime LN_DEFAULT_ARG(0) );

	/**
		@brief	  メモリ上の音声データから BGS を演奏する
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
		@param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayBGSMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100), int fadeTime LN_DEFAULT_ARG(0));

	/**
		@brief	  BGS の演奏を停止する
		@param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_StopBGS(int fadeTime LN_DEFAULT_ARG(0));

	/**
		@brief	  ME を演奏する
		@param[in]  filePath	: ファイル名
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayME( const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100) );

	/**
		@brief	  メモリ上の音声データから ME を演奏する
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlayMEMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief	  ME の演奏を停止する
	*/
	LN_STATIC_API
	void LNAudio_StopME();

	/**
		@brief	  SE を演奏する
		@param[in]  filePath	: ファイル名
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE( const LNChar* filePath, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100) );
	/*Option
	@override[hsp]
		test
	@override_end
	Option*/

	/**
		@brief	  SE を演奏する (3D サウンド)
		@param[in]  filePath	: ファイル名
		@param[in]  position	: 3D 空間上の座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));
	/*Option
	@override[hsp]
		test
	@override_end
	Option*/

	/**
		@brief	  SE を演奏する (3D サウンド)
		@param[in]  filePath	: ファイル名
		@param[in]  x			: 3D 空間上の X 座標
		@param[in]  y			: 3D 空間上の Y 座標
		@param[in]  z			: 3D 空間上の Z 座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3DXYZ( const LNChar* filePath, float x, float y, float z, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100) );
	/*Option
	@override[hsp]
		test
	@override_end
	Option*/

	/**
		@brief	  メモリ上の音声データから SE を演奏する
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySEMem(const void* data, int dataSize, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief	  メモリ上の音声データから SE を演奏する (3D サウンド)
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  position	: 3D 空間上の座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief	  メモリ上の音声データから SE を演奏する (3D サウンド)
		@param[in]  data		: メモリ上の音声ファイルデータ
		@param[in]  dataSize	: データサイズ (バイト単位)
		@param[in]  x			: 3D 空間上の X 座標
		@param[in]  y			: 3D 空間上の Y 座標
		@param[in]  z			: 3D 空間上の Z 座標
		@param[in]  distance	: 減衰距離
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  pitch		: ピッチ (50 ～ 200)
	*/
	LN_STATIC_API
	LNResult LNAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, int volume LN_DEFAULT_ARG(100), int pitch LN_DEFAULT_ARG(100));

	/**
		@brief	  すべての SE の演奏を停止する
	*/
	LN_STATIC_API
	void LNAudio_StopSE();

	/**
		@brief	  3D 空間の1メートル相当の距離の設定
		@param[in]  distance	: 距離
	*/
	LN_STATIC_API
	LNResult LNAudio_SetMetreUnitDistance(float distance);

	/**
		@brief	 再生中のBGMの音量を設定する (フェードアウト中は無効)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_SetBGMVolume(int volume, int fadeTime LN_DEFAULT_ARG(0));

	/**
		@brief	  再生中のBGSの音量を設定する (フェードアウト中は無効)
		@param[in]  volume		: ボリューム (0 ～ 100)
		@param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	*/
	LN_STATIC_API
	LNResult LNAudio_SetBGSVolume(int volume, int fadeTime LN_DEFAULT_ARG(0));


LN_CLASS_END

//==============================================================================
/**
	@brief		3D音声のリスナーに関する情報を表します。
*/
LN_STATIC_CLASS(LNSoundListener)

	/**
		@brief		リスナーの位置の設定
		@param[in]	position	: 3D 空間上の座標
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNSoundListener_SetPosition(const LNVector3* position);

	/**
		@brief		リスナーの位置の設定
		@param[in]	x			: 3D 空間上の X 座標
		@param[in]	y			: 3D 空間上の Y 座標
		@param[in]	z			: 3D 空間上の Z 座標
	*/
	LN_STATIC_API
	LN_PROPERTY
	void LNSoundListener_SetPositionXYZ( float x, float y, float z );

	/**
		@brief		リスナーの正面方向の設定
		@param[in]	direction		: 向き
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNSoundListener_SetDirection(const LNVector3* direction);

	/**
		@brief		リスナーの正面方向の設定
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	LN_STATIC_API
	LN_PROPERTY
	void LNSoundListener_SetDirectionXYZ(float x, float y, float z);

	/**
		@brief		リスナーの上方向の設定 (正面方向とは直交であること)
		@param[in]	direction	: 上方向
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNSoundListener_SetUpDirection(const LNVector3* direction);

	/**
		@brief		リスナーの上方向の設定 (正面方向とは直交であること)
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	LN_STATIC_API
	LN_PROPERTY
	void LNSoundListener_SetUpDirectionXYZ( float x, float y, float z );

	/**
		@brief		リスナーの速度の設定
		@param[in]	velocity	: 速度
	*/
	LN_STATIC_API
	LN_PROPERTY
	LNResult LNSoundListener_Velocity(const LNVector3* velocity);

	/**
		@brief	  リスナーの速度の設定
		@param[in]	x			: 速度の X 成分
		@param[in]	y			: 速度の Y 成分
		@param[in]	z			: 速度の Z 成分
	*/
	LN_STATIC_API
	LN_PROPERTY
	void LNSoundListener_VelocityXYZ( float x, float y, float z );


LN_CLASS_END
	
//==============================================================================
/**
	@brief		音声データひとつ分を表し、再生などの操作を行うクラスです。
*/
LN_CLASS(LNSound)

	/**
		@brief		ファイルから音声を作成する
		@param[out]	sound		: sound
		@param[in]	filePath	: ファイル名
		@param[in]	enable3d	: true の場合、3D 音源として作成する
		@param[in]	playType	: 音声の再生方法
	*/
	LN_INSTANCE_API
	LNResult LNSound_Create(LN_HANDLE(LNSound)* sound, const LNChar* filePath, LNBool enable3d LN_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LN_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO) );
	
	
	/**
		@brief		メモリ上の音声ファイルデータから音声を作成する
		@param[out]	sound		: sound
		@param[in]	data		: メモリ上の音声データへのポインタ
		@param[in]	dataSize	: データサイズ (バイト単位)
		@param[in]	enable3d	: true の場合、3D 音源として作成する
	*/
	LN_INSTANCE_API
	LNResult LNSound_CreateMem(LN_HANDLE(LNSound)* sound, const void* data, int dataSize, LNBool enable3d LN_DEFAULT_ARG(LN_FALSE));

	/**
		@brief		ボリュームの設定
		@param[in]	sound		: サウンドハンドル
		@param[in]	volume		: ボリューム (0～100)
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetVolume(LN_HANDLE(LNSound) sound, int volume);

	/**
		@brief		ボリュームの取得
		@param[in]	sound		: サウンドハンドル
		@param[out]	volume		: ボリュームを格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetVolume(LN_HANDLE(LNSound) sound, int* volume);

	/**
		@brief		ピッチの設定
		@param[in]	sound		: サウンドハンドル
		@param[in]	pitch		: ピッチ (50～200)
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetPitch(LN_HANDLE(LNSound) sound, int pitch);

	/**
		@brief		ピッチの取得
		@param[in]	sound		: サウンドハンドル
		@param[out]	pitch		: ピッチを格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetPitch(LN_HANDLE(LNSound) sound, int* pitch);

	/**
		@brief		ループ再生の有効設定
		@param[in]	sound		: サウンドハンドル
		@param[in]	loopEnable	: LN_TRUE = ループ再生する / LN_FALSE = しない
		@param[in]	begin		: ループ領域の先頭位置 (サンプル数単位)
		@param[in]	length		: ループ領域長さ (サンプル数単位)
		@details	begin と length に 0 を指定すると、全体をループ領域として設定します。
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetLoopState(LN_HANDLE(LNSound) sound, LNBool loopEnable, int begin LN_DEFAULT_ARG(0), int length LN_DEFAULT_ARG(0));

	/**
		@brief		ループ再生が有効かを調べる
		@param[in]	sound		: サウンドハンドル
		@param[out]	enabled		: ループ再生の有無状態を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_IsLoop(LN_HANDLE(LNSound) sound, LNBool* enabled);

	/**
		@brief		サウンドの再生状態を取得する
		@param[in]	sound		: サウンドハンドル
		@param[out]	state		: 状態を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetPlayState(LN_HANDLE(LNSound) sound, LNSoundPlayingState* state);

	/**
		@brief		音声を再生する
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Play(LN_HANDLE(LNSound) sound);

	/**
		@brief		再生を停止する
		@param[in]	sound		: サウンドハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSound_Stop(LN_HANDLE(LNSound) sound);

	/**
		@brief		一時停止
		@param[in]	sound		: サウンドハンドル
		@param[in]	pause		: LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除
	*/
	LN_INSTANCE_API
	LNResult LNSound_Pause(LN_HANDLE(LNSound) sound, LNBool pause);

	/**
		@brief		音量のフェード
		@param[in]	sound			: サウンドハンドル
		@param[in]	targetVolume	: 変更先の音量
		@param[in]	time			: フェードにかける時間 (ミリ秒)
		@param[in]	fadeState		: フェード完了後の動作の指定
	*/
	LN_INSTANCE_API
	LNResult LNSound_FadeVolume(LN_HANDLE(LNSound) sound, int targetVolume, int time, LNSoundFadeState fadeState);

	/**
		@brief		再生したサンプル数の取得
		@param[in]	sound		: サウンドハンドル
		@param[out]	samples		: 再生したサンプル数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetUnitsPlayed(LN_HANDLE(LNSound) sound, int* samples);

	/**
		@brief		音声データ全体のサンプル数の取得
		@param[in]	sound		: サウンドハンドル
		@param[out]	samples		: 音声データ全体のサンプル数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetTotalUnits(LN_HANDLE(LNSound) sound, int* samples);

	/**
		@brief		サンプリング周波数の取得
		@param[in]	sound		: サウンドハンドル
		@param[out]	frequency	: サンプリング周波数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_GetUnitsParSecond(LN_HANDLE(LNSound) sound, int* frequency);

	/**
		@brief		3D 音源かを調べる
		@param[in]	sound		: サウンドハンドル
		@param[out]	enabled		: 3D 音源かを示す値を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_Is3DSound(LN_HANDLE(LNSound) sound, LNBool* enabled);

	/**
		@brief		3D 音源としての位置を設定する
		@param[in]	sound		: サウンドハンドル
		@param[in]	position	: 3D 空間上の座標
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetEmitterPosition(LN_HANDLE(LNSound) sound, const LNVector3* position);

	/**
		@brief		3D 音源としての位置を設定する
		@param[in]	sound		: サウンドハンドル
		@param[in]	x			: 3D 空間上の X 座標
		@param[in]	y			: 3D 空間上の Y 座標
		@param[in]	z			: 3D 空間上の Z 座標
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetEmitterPositionXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z);

	/**
		@brief		3D 音源としての速度を設定する
		@param[in]	sound		: サウンドハンドル
		@param[in]	velocity	: 速度
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetEmitterVelocity(LN_HANDLE(LNSound) sound, const LNVector3* velocity);

	/**
		@brief		3D 音源としての速度を設定する
		@param[in]	sound		: サウンドハンドル
		@param[in]	x			: 速度の X 成分
		@param[in]	y			: 速度の Y 成分
		@param[in]	z			: 速度の Z 成分
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetEmitterVelocityXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z);

	/**
		@brief		3D 音源の減衰距離 (聴こえなくなる距離) を設定する
		@param[in]	sound		: サウンドハンドル
		@param[in]	distance	: 距離
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNSound_SetEmitterDistance(LN_HANDLE(LNSound) sound, float distance);

LN_CLASS_END
	
LN_MODULE_END

} // extern "C"
