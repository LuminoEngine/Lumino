#pragma once
#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_engine エンジン
	@brief		ライブラリの初期化や更新、終了処理等のアプリケーション全体に関わる機能です。
	@{
*/
LN_MODULE(Engine)

//==============================================================================
/**
	@brief		初期設定を行います。
*/
LN_STATIC_CLASS(LNConfig)

	/**
		@brief		デバッグ用のログファイルの出力有無を設定します。(既定値:LN_FALSE)
		@param[in]	enabled	: LN_TRUE:出力する / LN_FALSE:出力しない
	*/
	LN_STATIC_API
	void LNConfig_SetApplicationLogEnabled(LNBool enabled);

	/**
		@brief		標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(既定値:LN_FALSE)
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
		@brief		ユーザー定義のウィンドウハンドルを設定します。(既定値:NULL)
		@param[in]	windowHandle　: ユーザー定義のウィンドウハンドル
	*/
	LN_STATIC_API
	void LNConfig_SetUserWindowHandle(intptr_t windowHandle);

	/**
		@brief		サウンドオブジェクトのキャッシュサイズの設定
		@param[in]	objectCount	: キャッシュできるサウンドオブジェクトの最大数 (既定値:32)
		@param[in]	memorySize	: サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (既定値:0)
		@details	objectCount が 0 の場合、キャッシュを使用しません。
					memorySize が 0 の場合、メモリ使用量に制限を設けません。
	*/
	LN_STATIC_API
	void LNConfig_SetSoundCacheSize(int objectCount, int memorySize);
	
	/**
		@brief		DirectMusic の初期化方法を設定します。(既定値:LN_DIRECTMUSICMODE_NOT_USE)
		@param[in]	mode	: DirectMusic の初期化方法
		@details	DirectMusic の初期化には比較的時間がかかります。
					これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
					なお、DirectMusic を使用するためには LNConfig_SetUserWindowHandle でウィンドウハンドルを設定する必要があります。
	*/
	LN_STATIC_API
		void LNConfig_SetDirectMusicMode(LNDirectMusicMode mode);

	/**
		@brief		DirectMusic のリバーブエフェクトの強さを設定します。(既定値:0.75)
		@param[in]	level		: リバーブの強さ (0.0 ～ 1.0)
	*/
	LN_STATIC_API
	void LNConfig_SetDirectMusicReverbLevel(float level);

LN_CLASS_END

//==============================================================================
/**
	@brief		エンジン全体の初期化や更新等、包括的な処理を行うクラスです。
*/
LN_STATIC_CLASS(LNEngine)

	/**
		@brief		ライブラリを初期化します。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_INITIALIZER
	LNResult LNEngine_Initialize();

	/**
		@brief		ライブラリを初期化します。音声機能のみを使用する場合に呼び出します。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_INITIALIZER
	LNResult LNEngine_InitializeAudio();
	
	/**
		@brief		1フレーム分の更新処理を行います。
		@details	この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
	*/
	LN_STATIC_API
	LNResult LNEngine_UpdateFrame();
	
	/**
		@brief		アプリケーションを終了するべきかを確認します。
		@param[out]	outRequested	: 終了要求の有無を格納する変数
		@details	ウィンドウのクローズボタンが押された場合等、
					アプリケーションを終了するべき時には LN_FALSE を返します。
	*/
	LN_STATIC_API
	LNResult LNEngine_IsEndRequested(LNBool* outRequested);

	/**
		@brief		ライブラリの終了処理を行います。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_TERMINATOR
	void LNEngine_Terminate();

LN_CLASS_END

//==============================================================================
/**
	@brief	バージョン情報です。
*/
LN_STATIC_CLASS(LNVersion)

	/**
		@brief		メジャーバージョンを取得します。
		@param[out]	outMajor	: バージョン番号を格納する変数のポインタ
	*/
	LN_STATIC_API
	void LNVersion_GetMajor(int* outMajor);

	/**
		@brief		マイナーバージョンを取得します。
		@param[out]	outMinor	: バージョン番号を格納する変数のポインタ
	*/
	LN_STATIC_API
	void LNVersion_GetMinor(int* outMinor);

	/**
		@brief		リビジョンバージョンを取得します。
		@param[out]	outRevision	: バージョン番号を格納する変数のポインタ
	*/
	LN_STATIC_API
	void LNVersion_GetRevision(int* outRevision);
	
	/**
		@brief		ビルドバージョンを取得します。
		@param[out]	outBuild	: バージョン番号を格納する変数のポインタ
	*/
	LN_STATIC_API
	void LNVersion_GetBuild(int* outBuild);

	/**
		@brief		バージョン文字列を取得します。
		@param[out]	outStr	: 文字列へのポインタを格納する変数のポインタ
	*/
	LN_STATIC_API
	void LNVersion_GetString(const LNChar** outStr);

	/**
		@brief		指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
		@param[in]	major		: メジャーバージョン
		@param[in]	minor		: マイナーバージョン
		@param[in]	revision	: リビジョンバージョン
		@param[out]	outResult	: 結果を格納する変数のポインタ
		@details	指定バージョン >= コンパイルバージョン である場合、LN_TRUE となります。
	*/
	LN_STATIC_API
	void LNVersion_IsAtLeast(int major, int minor, int revision, LNBool* outResult);

LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
