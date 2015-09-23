
#pragma once
#include "LNCommon.h"
#include "LNTypedef.h"

extern "C" {

/**
	@brief	LightNote の初期化やフレーム更新、終了処理等のアプリケーション全体に関わる機能です。
*/
LN_MODULE(Application)

//==============================================================================
/**
	@brief		初期設定を行います。
*/
LN_STATIC_CLASS(LNConfig)

	/**
		@brief		デバッグ用のログファイルの出力有無を設定します。(初期値:LN_FALSE)
		@param[in]	enabled	: LN_TRUE:出力する / LN_FALSE:出力しない
	*/
	LN_STATIC_API
	void LNConfig_SetApplicationLogEnabled(LNBool enabled);

	/**
		@brief		標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:LN_FALSE)
		@param[in]	enabled	: LN_TRUE:割り当てる / LN_FALSE:割り当てない
	*/
	LN_STATIC_API
	void LNConfig_SetConsoleEnabled(LNBool enabled);

	/**
		@brief		ファイルを開くときにアクセスする暗号化アーカイブを登録します。
		@param[in]	filePath	: アーカイブファイルパス
		@param[in]	password	: アーカイブファイルを開くためのパスワード
	*/
	LN_STATIC_API
	void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password);

	/**
		@brief		ファイルへのアクセス優先順位を設定します。
		@param[in]	priority	: 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
	*/
	LN_STATIC_API
	void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority);

	/**
		@brief		ユーザー定義のウィンドウハンドルを設定します。(初期値:NULL)
		@param[in]	windowHandle　: ユーザー定義のウィンドウハンドル
	*/
	LN_STATIC_API
	void LNConfig_SetUserWindowHandle(intptr_t windowHandle);

	/**
		@brief		サウンドオブジェクトのキャッシュサイズの設定
		@param[in]	count		: キャッシュできるサウンドオブジェクトの最大数 (初期値:32)
		@param[in]	memorySize	: サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (初期値:0)
		@details	count が 0 の場合、キャッシュを使用しません。
					memorySize が 0 の場合、メモリ使用量に制限を設けません。
	*/
	LN_STATIC_API
	void LNConfig_SetSoundCacheSize(int count, int memorySize);
	
	/**
		@brief		DirectMusic の初期化方法を設定します。(初期値:LN_DIRECTMUSICMODE_NOT_USE)
		@param[in]	mode	: DirectMusic の初期化方法
		@details	DirectMusic の初期化には比較的時間がかかります。
					これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
	*/
	LN_STATIC_API
	void LNConfig_SetDirectMusicInitializeMode(LNDirectMusicMode mode);

	/**
		@brief		DirectMusic のリバーブエフェクトの強さを設定します。(初期値:70)
		@param[in]	level		: リバーブの強さ (0 ～ 100)
	*/
	LN_STATIC_API
	void LNConfig_SetDirectMusicReverbLevel(int level);

LN_CLASS_END

//==============================================================================
/**
	@brief		ライブラリ全体の初期化や更新等、包括的な処理を行うクラスです。
*/
LN_STATIC_CLASS(LNApplication)

	/**
		@brief		アプリケーションを初期化します。音声機能のみを使用する場合に呼び出します。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_INITIALIZER
	LNResult LNApplication_InitializeAudio();


	/**
		@brief		LightNote の終了処理を行います。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_TERMINATOR
	void LNApplication_Finalize();

LN_CLASS_END

LN_MODULE_END

} // extern "C"
