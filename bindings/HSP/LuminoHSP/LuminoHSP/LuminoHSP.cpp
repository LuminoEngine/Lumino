#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../hsp3plugin/hsp3plugin.h"
#include "../hsp3plugin/hspwnd.h"
#include "../../../src/C_API/LuminoC.h"
#include "LuminoHSP.h"

void* retValPtr = NULL;

//-----------------------------------------------------------------------------
// 命令実行時に呼び出される
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// 組み込み変数または関数実行時に呼ばれる
//-----------------------------------------------------------------------------
static void *reffunc(int *type_res, int cmd)
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	if (!Structs_reffunc(cmd, type_res, &retValPtr))
	{
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('で終わるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	return retValPtr;
}

//-----------------------------------------------------------------------------
// HSP 終了時に呼び出される
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Terminate();
	return 0;
}

//-----------------------------------------------------------------------------
// HSP 用プラグインのエントリーポイント
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

	// 構造体型の登録
	RegisterStructTypes(info);
}

