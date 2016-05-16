
#include "LNInternal.h"
#include <Lumino/Version.h>
#include "../include/LNApplication.h"

extern "C" {

//==============================================================================
// LNApplication
//==============================================================================

//------------------------------------------------------------------------------
void LNConfig_SetApplicationLogEnabled(LNBool enabled)
{
	LFManager::ConfigData.applicationLogEnabled = LNC_TO_BOOL(enabled);
}

//------------------------------------------------------------------------------
void LNConfig_SetConsoleEnabled(LNBool enabled)
{
	LFManager::ConfigData.ConsoleEnabled = LNC_TO_BOOL(enabled);
}

//------------------------------------------------------------------------------
void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password)
{
	EngineSettings::ArchiveFileEntry e;
	e.filePath = filePath;
	e.password = password;
	LFManager::ConfigData.ArchiveFileEntryList.Add(e);
}

//------------------------------------------------------------------------------
void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority)
{
	LFManager::ConfigData.FileAccessPriority = (FileAccessPriority)priority;
}

//------------------------------------------------------------------------------
void LNConfig_SetUserWindowHandle(intptr_t windowHandle)
{
	LFManager::ConfigData.userMainWindow = (void*)windowHandle;
}

//------------------------------------------------------------------------------
void LNConfig_SetSoundCacheSize(int objectCount, int memorySize)
{
	LFManager::ConfigData.soundCacheCapacity.objectCount = objectCount;
	LFManager::ConfigData.soundCacheCapacity.memorySize = memorySize;
}

//------------------------------------------------------------------------------
void LNConfig_SetDirectMusicMode(LNDirectMusicMode mode)
{
	LFManager::ConfigData.directMusicMode = (DirectMusicMode)mode;
}

//------------------------------------------------------------------------------
void LNConfig_SetDirectMusicReverbLevel(float level)
{
	LFManager::ConfigData.DirectMusicReverbLevel = level;
}

//==============================================================================
// LNApplication
//==============================================================================


#if 0
	/**
		@brief		アプリケーションを初期化します。
		@details	ライブラリのすべての機能を使用できるように初期化を行います。
	*/
	LN_STATIC_API
	LN_ATTR_LIBRARY_INITIALIZER
	LNResult LNApplication_Initialize();

//------------------------------------------------------------------------------
LNResult LNApplication_Initialize()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Engine->Initialize();
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}
#endif

//------------------------------------------------------------------------------
LNResult LNApplication_Initialize()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Engine->Initialize();
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNApplication_InitializeAudio()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Engine->InitializeAudioManager();
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

#endif
//------------------------------------------------------------------------------
LNResult LNApplication_UpdateFrame()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->UpdateFrame();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
//LNResult LNApplication_ResetFrameDelay()
//{
//	LFManager::Engine->ResetFrameDelay();
//	return ::LN_OK;
//}

//------------------------------------------------------------------------------
LNResult LNApplication_IsEndRequested(LNBool* requested)
{
	*requested = LNOTE_BOOL_TO_LNBOOL(LFManager::Engine->IsEndRequested());
	return ::LN_OK;
}

//------------------------------------------------------------------------------
// Note: Finalize という名前から変更した。Finalize は .NET で特殊な名前となってしまう。
//------------------------------------------------------------------------------
void LNApplication_Terminate()
{
	LFManager::Terminate();
}

//LN_INTERNAL_API void* LNApplication_GetInternalObject();
//------------------------------------------------------------------------------
//void* LNApplication_GetInternalObject()
//{
//	return LFManager::CoreManager;
//}



//==============================================================================
// NVersion
//==============================================================================

//------------------------------------------------------------------------------
void LNVersion_GetMajor(int* outMajor)
{
	if (outMajor != nullptr) {
		*outMajor = Version::Engine::GetMajor();
	}
}

//------------------------------------------------------------------------------
void LNVersion_GetMinor(int* outMinor)
{
	if (outMinor != nullptr) {
		*outMinor = Version::Engine::GetMinor();
	}
}

//------------------------------------------------------------------------------
void LNVersion_GetRevision(int* outRevision)
{
	if (outRevision != nullptr) {
		*outRevision = Version::Engine::GetRevision();
	}
}

//------------------------------------------------------------------------------
void LNVersion_GetBuild(int* outBuild)
{
	if (outBuild != nullptr) {
		*outBuild = Version::Engine::GetBuild();
	}
}

//------------------------------------------------------------------------------
void LNVersion_GetString(const LNChar** outStr)
{
	if (outStr != nullptr) {
		*outStr = Version::Engine::GetString();
	}
}

//------------------------------------------------------------------------------
void LNVersion_IsAtLeast(int major, int minor, int revision, LNBool* outResult)
{
	if (outResult != nullptr) {
		*outResult = LNC_TO_LNBOOL(Version::Engine::IsAtLeast(major, minor, revision));
	}
}

} // extern "C"
