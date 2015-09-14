// lnote_hsp.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "hsp3plugin/hsp3plugin.h"
#include <lnote.h>

//#include "stdafx.h"
//#include "lnote_hsp.h"
//
//
//// これは、エクスポートされた変数の例です。
//LNOTE_HSP_API int nlnote_hsp=0;
//
//// これは、エクスポートされた関数の例です。
//LNOTE_HSP_API int fnlnote_hsp(void)
//{
//	return 42;
//}
//
//// これは、エクスポートされたクラスのコンストラクターです。
//// クラス定義に関しては lnote_hsp.h を参照してください。
//Clnote_hsp::Clnote_hsp()
//{
//	return;
//}


int cmd_LNApplication_InitializeAudio()
{
	return LNApplication_InitializeAudio();
}
int cmd_LNAudio_PlayBGM()
{
	std::string p1 = code_gets();
	int p2 = code_getdi(100);
	int p3 = code_getdi(100);
	double p4 = code_getdd(0.0);
	return LNAudio_PlayBGM(p1.c_str(), p2, p3, p4);
}

static int cmdfunc(int cmd)
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch (cmd) {							// サブコマンドごとの分岐

	case 0x00:								// newcmd

		p1 = code_getdi(123);		// 整数値を取得(デフォルト123)
		stat = p1;					// システム変数statに代入
		break;

	case 0x01:								// newcmd2
		stat = cmd_LNApplication_InitializeAudio();
		break;

	case 0x02:								// newcmd3
		stat = cmd_LNAudio_PlayBGM();
		break;

	case 0x03:								// newcmd4
		//newcmd4();
		break;

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Terminate();
	return 0;
}

//-----------------------------------------------------------------------------
// HSP 用プラグインのエントリーポイント
//-----------------------------------------------------------------------------
EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO *info)
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init(info);			// SDKの初期化(最初に行なって下さい)
	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = NULL;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	/*
	//	イベントコールバックを発生させるイベント種別を設定する
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// イベント関数(eventfunc)の登録
	*/
}
