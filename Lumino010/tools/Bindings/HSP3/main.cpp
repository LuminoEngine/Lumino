
#include <LuminoEngine.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>
#include "../../../src/LuminoEngine/src/Engine/EngineManager.hpp"
#include "../../../src/LuminoEngine/src/Engine/EngineDomain.hpp"
#include "LuminoHSP.h"

extern bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr);
extern bool Commands_cmdfunc(int cmd, int* retVal);
extern void RegisterTypes(HSP3TYPEINFO* info);

static int cmdfunc(int cmd)
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	int ret = 0;
	if (Commands_cmdfunc(cmd, &ret)) {
		return ret;
	}

	puterror(HSPERR_UNSUPPORTED_FUNCTION);
	return RUNMODE_RUN;
}

static void* reffunc(int* type_res, int cmd)
{

	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	void* retValPtr;
	if (!Structs_reffunc(cmd, type_res, &retValPtr))
	{
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('で終わるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	//*type_res = HSPVAR_FLAG_INT;			// 返値のタイプを整数に指定する
	return retValPtr;
}

static int termfunc(int option)
{
	LNLog_Write(LN_LOG_LEVEL_DEBUG, u"HSP3", u"termfunc() started.");
	LNEngine_Terminate();
	LNRuntime_Finalize();
	LNLog_Write(LN_LOG_LEVEL_DEBUG, u"HSP3", u"termfunc() finished.");
	return 0;
}

static void OnPreInitEngineManager()
{
	if (!ln::detail::EngineManager::s_settings.userMainWindow) {
		BMSCR* bm = (BMSCR*)getbmscr(active_window);
		ln::detail::EngineManager::s_settings.userMainWindow = reinterpret_cast<intptr_t>(bm->hwnd);
	}
}

static void OnPostInitEngineManager()
{
	ln::detail::EngineDomain::engineManager()->preUpdateCallback = []() {
		// update から抜けて HSP3 Runtime へ制御を返すとき、await を呼んでもらうようにする。
		// こうしておかないと HSP3 として必要なメッセージディスパッチが呼ばれない。
		ctx->waitcount = p1;
		ctx->waittick = -1;
		ctx->runmode = RUNMODE_AWAIT;
		ctx->msgfunc(ctx);
	};
}

//-----------------------------------------------------------------------------
// Entry point for HSP plugin.
//-----------------------------------------------------------------------------
EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO* info)
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init(info);			// SDKの初期化(最初に行なって下さい)
	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	/*
	//	イベントコールバックを発生させるイベント種別を設定する
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// イベント関数(eventfunc)の登録
	*/

	RegisterTypes(info);

	// Lumino のランタイムを初期化し、Engine 初期化前に Application を作れるようにする。
	LNRuntimeSettings settings;
	settings.runtimeFinalizedCallback = nullptr;
	settings.referenceCountTrackerCallback = nullptr;
	settings.runtimeGetTypeInfoIdCallback = nullptr;
	LNRuntime_Initialize(&settings);

	ln::Runtime::setAStringEncoding(ln::TextEncoding::systemMultiByteEncoding());

	ln::EngineContext::current()->engineManagerPreInit = OnPreInitEngineManager;
	ln::EngineContext::current()->engineManagerPostInit = OnPostInitEngineManager;
	

	// NOTE: メインウィンドウの HWND は active_window マクロでとれるが、この時点では NULL なので注意。
	// プラグインロード時にメインウィンドウにアタッチすることはできない。
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

