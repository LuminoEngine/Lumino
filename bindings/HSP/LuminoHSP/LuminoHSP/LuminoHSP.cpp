// LuminoHSP.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../hsp3plugin/hsp3plugin.h"
#include "../../../src/C_API/LuminoC.h"

#include "LuminoHSP.h"


#define TRACE printf
#if 0
#define sbAlloc hspmalloc
#define sbFree hspfree


//#define LNVector3 float

/*
変数バッファ(バイナリ)のポインタとサイズを返します。
要素が可変長の場合は該当する１配列のバイナリのみが対象となり、
要素が固定長(int,double等)の場合は全配列バイナリを返します。
また、(*size)にメモリ確保サイズを返します。
*/
static void* hspCommon_GetBlockSize(PVal *pval, PDAT *pdat, int *size)
{
	TRACE("hspCommon_GetBlockSize\n");
	*size = pval->size - (((char *)pdat) - pval->pt);
	return (pdat);
}

static void hspCommon_AllocBlock(PVal *pval, PDAT *pdat, int size)
{
	TRACE("hspCommon_AllocBlock\n");
}





//static short* g_LNVector3_aftertype;
static int g_LNVector3_typeid = 0;



static int GetVarSize(PVal *pval)
{
	//		PVALポインタの変数が必要とするサイズを取得する
	//		(sizeフィールドに設定される)
	//
	int size;
	size = pval->len[1];
	if (pval->len[2]) size *= pval->len[2];
	if (pval->len[3]) size *= pval->len[3];
	if (pval->len[4]) size *= pval->len[4];
	size *= sizeof(LNVector3);
	return size;
}


/*
	pval変数が必要とするサイズを確保し初期化を行ないます。
	pval2がNULLの場合は、新規データになります。
	pval2が指定されている場合は、pval2の内容を継承して再確保を
	行なうことを示しています。この場合、pval2のsize、ptをもとに
	内容をコピーするようにしてください。また、ptの解放も同時に
	行なう必要があります。

	例:
		if ( pval2 != NULL ) {
			memcpy( pt, pval->pt, pval->size );
			free( pval->pt );
		}
*/
static void hspLNVector3_Alloc(PVal *pval, const PVal *pval2)
{
	TRACE("hspLNVector3_Alloc\n");
	//		pval変数が必要とするサイズを確保する。
	//		(pvalがすでに確保されているメモリ解放は呼び出し側が行なう)
	//		(flagの設定は呼び出し側が行なう)
	//		(pval2がNULLの場合は、新規データ)
	//		(pval2が指定されている場合は、pval2の内容を継承して再確保)
	//
	//int i;
#if 0
	if (pval->len[1] < 1) pval->len[1] = 1;		// 配列を最低1は確保する
	int size = GetVarSize(pval);//sizeof(LNVector3) * pval->len[1];//GetVarSize(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	char* pt = sbAlloc(size);
	LNVector3* fv = (LNVector3 *)pt;
	memset(fv, 0, size);
	//for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { fv[i] = 0.0; }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
#endif

	int i, size;
	char *pt;
	LNVector3 *fv;
	if (pval->len[1] < 1) pval->len[1] = 1;		// 配列を最低1は確保する
	size = GetVarSize(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	pt = sbAlloc(size);
	fv = (LNVector3 *)pt;
	for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { /*fv[i] = 0.0;*/ }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
	TRACE("hspLNVector3_Alloc e\n");
}

/*
pvalが確保しているメモリを解放します。
多くの場合、HSP3コアが自動的に呼び出しを行ないます。
*/
static void hspLNVector3_Free(PVal *pval)
{
	TRACE("hspLNVector3_Free\n");
	//		PVALポインタの変数メモリを解放する
	//
	if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}

/*
PValが示すデータの実態がある先頭ポインタを返します。
(PDAT*は、実態データのポインタを示しています。)
配列変数をサポートしている場合は、オフセット情報(PVal->offset)を
もとにデータの実態ポインタを求める必要があります。
*/
static PDAT* hspLNVector3_GetPtr(PVal *pval)
{
	TRACE("hspLNVector3_GetPtr\n");
	return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
	//return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
}

static void* hspLNVector3_Cnv(const void *buffer, int flag)
{
	TRACE("hspLNVector3_Cnv\n");
	throw HSPVAR_ERROR_TYPEMISS;
	return (void *)buffer;
}

static void* hspLNVector3_CnvCustom(const void *buffer, int flag)
{
	TRACE("hspLNVector3_CnvCustom\n");
	throw HSPVAR_ERROR_TYPEMISS;
	return (void *)buffer;
}

/*
pdatが示す内容のサイズ(バイト数)を返します。
この関数は、１要素のデータサイズが可変長(basesizeが-1)の時に
呼び出されます。通常は、basesizeと同じサイズを戻しておいて
下さい。
*/
static int hspLNVector3_GetSize(const PDAT *pdatl)
{
	TRACE("hspLNVector3_GetSize\n");
	return sizeof(LNVector3);
}

static int hspLNVector3_GetUsing(const PDAT *pdat)
{
	return 1;
}

#define GetPtr(pval) ((LNVector3 *)pval)
/*
	代入演算子
*/
static void hspLNVector3_Set(PVal *pval, PDAT *pdat, const void *in)
{
	TRACE("hspLNVector3_Set\n");
	*GetPtr(pval) = *((LNVector3*)(in));
	//printf("%f, %f, %f\n", ((LNVector3*)pval)->X, ((LNVector3*)pval)->Y, ((LNVector3*)pval)->Z);
}


//static int GetVarSize(PVal *pval)
//{
//	//		PVALポインタの変数が必要とするサイズを取得する
//	//		(sizeフィールドに設定される)
//	//
//	int size;
//	size = pval->len[1];
//	if (pval->len[2]) size *= pval->len[2];
//	if (pval->len[3]) size *= pval->len[3];
//	if (pval->len[4]) size *= pval->len[4];
//	size *= sizeof(float);
//	return size;
//}


EXPORT int hspLNVector3_typeid()
{
	TRACE("hspLNVector3_typeid\n");
	return g_LNVector3_typeid;
}

EXPORT void hspLNVector3_Init(HspVarProc *p)
{
	TRACE("hspLNVector3_Init\n");
	//aftertype = &p->aftertype;
	
	/*
	演算後にタイプが変更される演算子を持つ場合は、このフィールドで
	演算後のタイプ値をシステムに通知します。
	*/
	//g_LNVector3_aftertype = &p->aftertype;

	p->Set = hspLNVector3_Set;

	p->GetPtr = hspLNVector3_GetPtr;
	//p->Cnv = hspLNVector3_Cnv;
	//p->CnvCustom = hspLNVector3_CnvCustom;
	p->GetSize = hspLNVector3_GetSize;
	p->GetBlockSize = hspCommon_GetBlockSize;
	p->AllocBlock = hspCommon_AllocBlock;
	//p->GetUsing = hspLNVector3_GetUsing;

	p->Alloc = hspLNVector3_Alloc;
	p->Free = hspLNVector3_Free;


	//p->ArrayObject = HspVarVariant_ArrayObject;
	//p->ArrayObjectRead = HspVarVariant_ArrayObjectRead;
	//p->ObjectWrite = HspVarVariant_ObjectWrite;
	//p->ObjectMethod = HspVarVariant_ObjectMethod;

	//p->AddI = HspVarFloat_AddI;
	//p->SubI = HspVarFloat_SubI;
	//p->MulI = HspVarFloat_MulI;
	//p->DivI = HspVarFloat_DivI;
	//p->ModI = HspVarFloat_Invalid;
	//p->AndI = HspVarFloat_Invalid;
	//p->OrI  = HspVarFloat_Invalid;
	//p->XorI = HspVarFloat_Invalid;
	//p->EqI = HspVarFloat_EqI;
	//p->NeI = HspVarFloat_NeI;
	//p->GtI = HspVarFloat_GtI;
	//p->LtI = HspVarFloat_LtI;
	//p->GtEqI = HspVarFloat_GtEqI;
	//p->LtEqI = HspVarFloat_LtEqI;
	//p->RrI = HspVarFloat_Invalid;
	//p->LrI = HspVarFloat_Invalid;

	p->vartype_name = "LNVector3";				// タイプ名
	p->version = 0x001;					// 型タイプランタイムバージョン(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE | /*HSPVAR_SUPPORT_VARUSE | */HSPVAR_SUPPORT_FLEXARRAY;/* | HSPVAR_SUPPORT_FLEXARRAY*/;
	// サポート状況フラグ(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(LNVector3);		// １つのデータが使用するサイズ(byte) / 可変長の時は-1
	g_LNVector3_typeid = p->flag;
}

















//
//
//// これは、エクスポートされた変数の例です。
//LUMINOHSP_API int nLuminoHSP=0;
//
//// これは、エクスポートされた関数の例です。
//LUMINOHSP_API int fnLuminoHSP(void)
//{
//	return 42;
//}
//
//// これは、エクスポートされたクラスのコンストラクターです。
//// クラス定義に関しては LuminoHSP.h を参照してください。
//CLuminoHSP::CLuminoHSP()
//{
//	return;
//}











#endif


/*------------------------------------------------------------*/
/*
HSPVAR core interface (float)
*/
/*------------------------------------------------------------*/

#define GetPtr(pval) ((LNVector3 *)pval)
#define sbAlloc hspmalloc
#define sbFree hspfree

static int mytype;
//static float conv;
//static short *aftertype;
//static char custom[64];

// Core
static PDAT *HspVarFloat_GetPtr(PVal *pval)
{
	return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
}

//static void *HspVarFloat_Cnv(const void *buffer, int flag)
//{
//	//		リクエストされた型 -> 自分の型への変換を行なう
//	//		(組み込み型にのみ対応でOK)
//	//		(参照元のデータを破壊しないこと)
//	//
//	switch (flag) {
//	case HSPVAR_FLAG_STR:
//		conv = (float)atof((char *)buffer);
//		return &conv;
//	case HSPVAR_FLAG_INT:
//		conv = (float)(*(int *)buffer);
//		return &conv;
//	case HSPVAR_FLAG_DOUBLE:
//		conv = (float)(*(double *)buffer);
//		break;
//	default:
//		throw HSPVAR_ERROR_TYPEMISS;
//	}
//	return (void *)buffer;
//}


//static void *HspVarFloat_CnvCustom(const void *buffer, int flag)
//{
//	//		(カスタムタイプのみ)
//	//		自分の型 -> リクエストされた型 への変換を行なう
//	//		(組み込み型に対応させる)
//	//		(参照元のデータを破壊しないこと)
//	//
//	float p;
//	p = *(float *)buffer;
//	switch (flag) {
//	case HSPVAR_FLAG_STR:
//		sprintf(custom, "%f", p);
//		break;
//	case HSPVAR_FLAG_INT:
//		*(int *)custom = (int)p;
//		break;
//	case HSPVAR_FLAG_DOUBLE:
//		*(double *)custom = (double)p;
//		break;
//	default:
//		throw HSPVAR_ERROR_TYPEMISS;
//	}
//	return custom;
//}


static int GetVarSize2(PVal *pval)
{
	//		PVALポインタの変数が必要とするサイズを取得する
	//		(sizeフィールドに設定される)
	//
	int size;
	size = pval->len[1];
	if (pval->len[2]) size *= pval->len[2];
	if (pval->len[3]) size *= pval->len[3];
	if (pval->len[4]) size *= pval->len[4];
	size *= sizeof(LNVector3);
	return size;
}


static void HspVarFloat_Free(PVal *pval)
{
	//		PVALポインタの変数メモリを解放する
	//
	if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}


static void HspVarFloat_Alloc(PVal *pval, const PVal *pval2)
{
	//		pval変数が必要とするサイズを確保する。
	//		(pvalがすでに確保されているメモリ解放は呼び出し側が行なう)
	//		(flagの設定は呼び出し側が行なう)
	//		(pval2がNULLの場合は、新規データ)
	//		(pval2が指定されている場合は、pval2の内容を継承して再確保)
	//
	int i, size;
	char *pt;
	LNVector3 *fv;
	if (pval->len[1] < 1) pval->len[1] = 1;		// 配列を最低1は確保する
	size = GetVarSize2(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	pt = sbAlloc(size);
	fv = (LNVector3 *)pt;
	for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { /*fv[i] = 0.0;*/ }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
}

/*
static void *HspVarFloat_ArrayObject( PVal *pval, int *mptype )
{
//		配列要素の指定 (文字列/連想配列用)
//
throw HSPERR_UNSUPPORTED_FUNCTION;
return NULL;
}
*/

// Size
static int HspVarFloat_GetSize(const PDAT *pval)
{
	return sizeof(LNVector3);
}

// Set
static void HspVarFloat_Set(PVal *pval, PDAT *pdat, const void *in)
{
	*GetPtr(pdat) = *((LNVector3 *)(in));
	printf("%f, %f, %f\n", ((LNVector3*)pdat)->X, ((LNVector3*)pdat)->Y, ((LNVector3*)pdat)->Z);
}


static void *GetBlockSize(PVal *pval, PDAT *pdat, int *size)
{
	*size = pval->size - (((char *)pdat) - pval->pt);
	return (pdat);
}

static void AllocBlock(PVal *pval, PDAT *pdat, int size)
{
}


/*------------------------------------------------------------*/

EXPORT int HspVarFloat_typeid(void)
{
	return mytype;
}


EXPORT void HspVarFloat_Init(HspVarProc *p)
{
	//aftertype = &p->aftertype;

	p->Set = HspVarFloat_Set;
	//p->Cnv = HspVarFloat_Cnv;
	p->GetPtr = HspVarFloat_GetPtr;
	//p->CnvCustom = HspVarFloat_CnvCustom;
	p->GetSize = HspVarFloat_GetSize;
	p->GetBlockSize = GetBlockSize;
	p->AllocBlock = AllocBlock;

	//	p->ArrayObject = HspVarFloat_ArrayObject;
	p->Alloc = HspVarFloat_Alloc;
	p->Free = HspVarFloat_Free;

	//p->AddI = HspVarFloat_AddI;
	//p->SubI = HspVarFloat_SubI;
	//p->MulI = HspVarFloat_MulI;
	//p->DivI = HspVarFloat_DivI;
	//	p->ModI = HspVarFloat_Invalid;

	//	p->AndI = HspVarFloat_Invalid;
	//	p->OrI  = HspVarFloat_Invalid;
	//	p->XorI = HspVarFloat_Invalid;

	//p->EqI = HspVarFloat_EqI;
	//p->NeI = HspVarFloat_NeI;
	//p->GtI = HspVarFloat_GtI;
	//p->LtI = HspVarFloat_LtI;
	//p->GtEqI = HspVarFloat_GtEqI;
	//p->LtEqI = HspVarFloat_LtEqI;


	//	p->RrI = HspVarFloat_Invalid;
	//	p->LrI = HspVarFloat_Invalid;

	p->vartype_name = "LNVector3";				// タイプ名
	p->version = 0x001;					// 型タイプランタイムバージョン(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
	// サポート状況フラグ(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(LNVector3);		// １つのデータが使用するサイズ(byte) / 可変長の時は-1
	mytype = p->flag;
}

/*------------------------------------------------------------*/






//-----------------------------------------------------------------------------
// 命令実行時に呼び出される
//-----------------------------------------------------------------------------
static int cmdfunc(int cmd)
{
	TRACE("cmdfunc\n");
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch (cmd) {							// サブコマンドごとの分岐

	case 0x00:								// newcmd

		p1 = code_getdi(123);		// 整数値を取得(デフォルト123)
		stat = p1;					// システム変数statに代入
		break;

	//case 0x01:								// newcmd2
	//	stat = cmd_LNApplication_InitializeAudio();
	//	break;

	//case 0x02:								// newcmd3
	//	stat = cmd_LNAudio_PlayBGM();
	//	break;

	case 0x10:								// newcmd4
		//newcmd4();
	{
		std::string p1 = code_gets();
		PVal* p2;
		printf("p2\n");
		code_getva(&p2);
		printf("p2->flag: %d\n", p2->flag);
		if (p2->flag != HspVarFloat_typeid()) { throw HSPVAR_ERROR_TYPEMISS; }
		float p3 = code_getd();
		printf("call\n");
		stat = LNAudio_PlaySE3D(p1.c_str(), (LNVector3*)p2->pt, p3);
		break;
	}

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}

static LNVector3 retVal;
void* retValPtr = NULL;
//-----------------------------------------------------------------------------
// 組み込み変数または関数実行時に呼ばれる
//-----------------------------------------------------------------------------
static void *reffunc(int *type_res, int cmd)
{
	TRACE("reffunc s\n");
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();


	switch (cmd) {							// サブコマンドごとの分岐

	case 0x00:								// float関数
	{
		int r = code_getprm();
		printf("r:%d\n", r);
		double p1 = *((double*)mpval->pt);// code_getd();
		printf("r:%d\n", r);
		puterror(HSPERR_NO_DEFAULT);	// long jump
		printf("rf:%d\n", r);
		double p2 = code_getd();
		double p3 = code_getd();
		retVal.X = p1;
		retVal.Y = p2;
		retVal.Z = p3;
		retValPtr = &retVal;
		break;
	}
	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('で終わるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	TRACE("reffunc e\n");
	//*type_res = HspVarFloat_typeid();		// 返値のタイプを指定する
	//*type_res = hspLNVector3_typeid();
	*type_res = HspVarFloat_typeid();
	return retValPtr;//(void *)&ref_fval;
}

//-----------------------------------------------------------------------------
// HSP 終了時に呼び出される
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Finalize();
	printf("LNApplication_Finalize\n");
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
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	/*
	//	イベントコールバックを発生させるイベント種別を設定する
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// イベント関数(eventfunc)の登録
	*/


	LNConfig_SetConsoleEnabled(LN_TRUE);
	LNApplication_InitializeAudio();

	// 新しい型の追加
	registvar(-1, HspVarFloat_Init);
	//registvar(-1, hspLNVector3_Init);
	printf("hsp3cmdinit e\n");


	printf("LNApplication_InitializeAudio\n");
}

