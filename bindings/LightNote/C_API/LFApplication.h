
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	LightNote の初期化やフレーム更新、終了処理等のアプリケーション全体に関わる機能です。
*/
LN_MODULE(Application)

//==============================================================================
/**
	@brief		LightNote の初期化や更新等、包括的な処理を行うクラスです。
*/
LN_STATIC_CLASS(LNCore)
	
	/**
		@brief		LightNote を初期化します。
		@details	ライブラリのすべての機能を使用できるように初期化を行います。
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNCore_Initialize();
	/*Option
		@override[cpp]
		@override_end
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		フレームを更新します。
		@details	LightNote の状態を更新し、時間を1フレーム分進めます。
					この関数は必ず1フレームに1度だけ呼び出す必要があります。
	*/
	LN_STATIC_API
	LNResult LNCore_Update();
	/*Option
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		画面の更新タイミングをリセットします。
		@details	時間のかかる処理の後にこのメソッドを呼ぶことで、
					極端なフレームスキップが発生しないようにすることができます。
	*/
	LN_STATIC_API
	LNResult LNCore_ResetFrameDelay();

	/**
		@brief		アプリケーションを終了するべきかを確認します。
		@param[out]	requested	: 終了要求の有無を格納する変数
		@details	ウィンドウのクローズボタンが押された場合等、
					アプリケーションを終了するべき時には LN_FALSE を返します。
	*/
	LN_STATIC_API
	LNResult LNCore_IsEndRequested(LNBool* requested);

	/**
		@brief		LightNote の終了処理を行います。
	*/
	LN_STATIC_API
	LN_LIBRARY_TERMINATOR
	void LNCore_Terminate();


	LN_INTERNAL_API void* LNCore_GetInternalObject();

LN_CLASS_END

LN_MODULE_END

} // extern "C"
