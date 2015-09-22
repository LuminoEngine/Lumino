﻿
#include "LNInternal.h"
#include "LNApplication.h"

extern "C" {

//=============================================================================
// LNApplication
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetApplicationLogEnabled(LNBool enabled)
{
	LFManager::ConfigData.ApplicationLogEnabled = LNC_TO_BOOL(enabled);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetConsoleEnabled(LNBool enabled)
{
	LFManager::ConfigData.ConsoleEnabled = LNC_TO_BOOL(enabled);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password)
{
	ApplicationSettings::ArchiveFileEntry e;
	e.FilePath = filePath;
	e.Password = password;
	LFManager::ConfigData.ArchiveFileEntryList.Add(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetUserWindowHandle(void* windowHandle)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetSoundCacheSize(int count, int memorySize)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetDirectMusicInitializeMode(LNDirectMusicMode mode)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNConfig_SetDirectMusicReverbLevel(int level)
{

}

//=============================================================================
// LNApplication
//=============================================================================


#if 0
	/**
		@brief		アプリケーションを初期化します。
		@details	ライブラリのすべての機能を使用できるように初期化を行います。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_INITIALIZER
	LNResult LNApplication_Initialize();

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_Initialize()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Application->Initialize();
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_InitializeAudio()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Application->InitialzeAudioManager();
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}

#if 0

	/**
		@brief		フレームを更新します。
		@details	LightNote の状態を更新し、時間を1フレーム分進めます。
					この関数は必ず1フレームに1度だけ呼び出す必要があります。
	*/
	LN_STATIC_API
	LNResult LNApplication_Update();
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
	LNResult LNApplication_ResetFrameDelay();

	/**
		@brief		アプリケーションを終了するべきかを確認します。
		@param[out]	requested	: 終了要求の有無を格納する変数
		@details	ウィンドウのクローズボタンが押された場合等、
					アプリケーションを終了するべき時には LN_FALSE を返します。
	*/
	LN_STATIC_API
	LNResult LNApplication_IsEndRequested(LNBool* requested);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_Update()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Application->UpdateFrame();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_ResetFrameDelay()
{
	LFManager::Application->ResetFrameDelay();
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_IsEndRequested(LNBool* requested)
{
	*requested = LNOTE_BOOL_TO_LNBOOL(LFManager::Application->IsEndRequested());
	return ::LN_OK;
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNApplication_Finalize()
{
	LFManager::Finalize();
}

//LN_INTERNAL_API void* LNApplication_GetInternalObject();
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void* LNApplication_GetInternalObject()
//{
//	return LFManager::CoreManager;
//}

} // extern "C"
